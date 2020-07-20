// =========================================================
//      P2.cpp
// 
// ---------------------------------------------------------
// Author:      Axel Perez
// Date:        7/19/20
// =========================================================

#include "P2.hpp"
using namespace std;

// Stream Thread Function:
//      Thread function that receives stream data from user inputted socket
//      and adds user URL paths to the global Frequent Path-Traversal Patterns 
//      Tree. Terminates when stream data is finished or if global exit thread
//      flag is set.
void streamThread(string hostname_or_ip, int portno) {
    int bytesRecvd, tempUID;
    string temp, tempURL;
    unordered_map<int, vector<string>> userUrlPaths;    // paths for each user
    unordered_map<int, unordered_set<string>> userUrls; // set of urls for each user
    Client threadClient(hostname_or_ip, portno);
    if (threadClient.errorFlag) {       // if errorFlag set in client, end thread
        return;
    }

    threadClient.connectToServer();
    if (threadClient.errorFlag) {
        return;
    }

    do {
        stringstream buffStream;
        
        bytesRecvd = threadClient.receiveFromServer();
        if (threadClient.errorFlag) {
            return;
        }

        buffStream << threadClient.buffer;

        // if received data has more than one entry, loop for all entries
        while (getline(buffStream, temp)) {
            stringstream lineStream(temp);
            
            getline(lineStream, temp, ','); // seperate by comma
            tempUID = stoi(temp);
            getline(lineStream, temp, ',');
            tempURL = temp;
            cout << tempUID << ", " << tempURL << endl;

            // if new user, add key to set and vector
            if (userUrls.count(tempUID) == 0) {
                userUrls[tempUID] = unordered_set<string>({tempURL});
                userUrlPaths[tempUID] = vector<string>({tempURL});
            }
            else {
                // if new URL in path, add to path 
                if (userUrls[tempUID].count(tempURL) == 0) {
                    userUrls[tempUID].insert(tempURL);
                    userUrlPaths[tempUID].push_back(tempURL);
                }
                // if repeated URL, start new path
                else {
                    userUrls[tempUID] = unordered_set<string>({tempURL});
                    userUrlPaths[tempUID] = vector<string>({tempURL});
                }
            }
            // lock access to global tree
            treeMutex.lock();  
            globalFPTree.insert(userUrlPaths[tempUID]); // insert path into tree
            treeMutex.unlock();
        }
    }
    while (bytesRecvd != 0 && !exitThread);     // exit when stream finishes or exit thread flag is set
    
    close(threadClient.sockfd);
}

// Handle Input Function:
//      Receive both new sockets and adhoc queries from stdin with error handling.
//      For each new correctly formatted socket, create stream thread to recieve
//      stream data from server. For adhoc queries, print Maximum Forward References
//      with support threshold inputted.
void handleInput(vector<thread> &myThreads) {
    string line, host;
    int portno, suppThresh;
    map<int, vector<vector<string>>> mfrs;

    while(getline(cin, line)) {     // while inputted line is not EOF
        stringstream ss(line);

        if (line.find(",") != string::npos) {   // check to see if user input is a socket
            getline(ss, host, ',');     // get host/ip
            getline(ss, line, ',');     // get port number
            
            // Catch error as exception if portnumber is not an integer
            try {
                portno = stoi(line);
            }
            catch(...) {
                cout << "Input Format Error: Please enter integer as the port number in this format: \"hostname_or_ip, port_number\"\n";
                continue;
            }
            myThreads.push_back(thread(streamThread, host, portno));    // create stream thread and save thread to join later
        }
        else {
            // Catch error as exception if inputted format is missing "," or not a valid integer
            try {
                suppThresh = stoi(line);
                if (suppThresh < 1) {
                    throw -1;
                }
            }
            catch(...) {
                cout << "Input Format Error: Please enter an integer > 0 as the support threshold or a socket: \"hostname_or_ip, port_number\"\n";
                continue;
            }
            // Lock global tree when getting the Maximum Forward References
            treeMutex.lock();
            mfrs = globalFPTree.getMFRs(suppThresh);
            treeMutex.unlock();
            printMFRs(mfrs, suppThresh);    // print MFRs once unlocked
        }
    }
}

// Print Maximum Forward References Function:
//      Prints Maximum Forward References provided as input. Format is as specified in the assignment.
void printMFRs(map<int, vector<vector<string>>> mfrs, int suppThresh) {
    int count;

    coutMutex.lock();   // lock cout so print is not interrupted by stream data

    if (mfrs.size() == 0) {
        cout << "No frequent path-traversal patterns with support threshold " << suppThresh << endl;
    }

    for (auto &length: mfrs) {
        cout << "\nWith support " << suppThresh << ", the frequent path-traversal patterns with length " << length.first << " are:" << endl;
        count = 1;
        for (auto &path: length.second) {
            cout << count << ")\t";
            for (auto &url: path) {
                cout << url << ", ";
            }
            cout << endl << endl;
            count++;
        }
    }
    coutMutex.unlock();
}

int main(void) {
    vector<thread> myThreads;

    handleInput(myThreads);
    exitThread = true;  // Tell threads to return with atomic global flag
    // Join all created threads
    for (auto &thr: myThreads) {
        thr.join();
    }

    return 0;
}
