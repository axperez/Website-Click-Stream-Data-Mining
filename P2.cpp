// =========================================================
//      P2.cpp
// 
// ---------------------------------------------------------
// Author:      Axel Perez
// Date:        7/12/20
// =========================================================

#include "P2.hpp"
using namespace std;

void streamThread(string hostname_or_ip, int portno) {
    int bytesRecvd, tempUID;
    string temp, tempURL;
    unordered_map<int, vector<string>> userUrlPaths;
    unordered_map<int, unordered_set<string>> userUrls;
    Client threadClient(hostname_or_ip, portno);
    if (threadClient.errorFlag) {
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

        if (bytesRecvd > 0) {
            buffStream << threadClient.buffer;

            while (getline(buffStream, temp)) {
                stringstream lineStream(temp);
                
                getline(lineStream, temp, ',');
                tempUID = stoi(temp);
                getline(lineStream, temp, ',');
                tempURL = temp;
                cout << tempUID << ", " << tempURL << endl;

                if (userUrls.count(tempUID) == 0) {
                    userUrls[tempUID] = unordered_set<string>({tempURL});
                    userUrlPaths[tempUID] = vector<string>({tempURL});
                }
                else {
                    if (userUrls[tempUID].count(tempURL) == 0) {
                        userUrls[tempUID].insert(tempURL);
                        userUrlPaths[tempUID].push_back(tempURL);
                    }
                    else {
                        userUrls[tempUID] = unordered_set<string>({tempURL});
                        userUrlPaths[tempUID] = vector<string>({tempURL});
                    }
                }
                treeMutex.lock();
                globalFPTree.insert(userUrlPaths[tempUID]);
                treeMutex.unlock();
            }
        }
    }
    while (bytesRecvd != 0 && !exitThread);
    
    close(threadClient.sockfd);
}

void handleInput(vector<thread> &myThreads) {
    string line, host;
    int portno, suppThresh;
    map<int, vector<vector<string>>> mfrs;

    while(getline(cin, line)) {
        stringstream ss(line);

        if (line.find(",") != string::npos) {
            getline(ss, host, ',');
            //cout << host << ",";
            getline(ss, line, ',');
            //cout << line << endl;
            try {
                portno = stoi(line);
            }
            catch(...) {
                cout << "Input Format Error: Please enter integer as the port number in this format: \"hostname_or_ip, port_number\"\n";
                continue;
            }
            myThreads.push_back(thread(streamThread, host, portno));
        }
        else {
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
            treeMutex.lock();
            mfrs = globalFPTree.getMFRs(suppThresh);
            treeMutex.unlock();
            printMFRs(mfrs, suppThresh);
        }
    }
}

void printMFRs(map<int, vector<vector<string>>> mfrs, int suppThresh) {
    int count;

    coutMutex.lock();

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
    exitThread = true;
    for (auto &thr: myThreads) {
        thr.join();
    }

    return 0;
}
