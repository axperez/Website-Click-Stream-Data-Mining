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
    Client threadClient(hostname_or_ip, portno);
    unordered_map<int, vector<string>> userUrlPaths;
    unordered_map<int, unordered_set<string>> userUrls;

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
    while (bytesRecvd != 0);
    
    close(threadClient.sockfd);
}

int main(void) {
    thread th1(streamThread, "linux10617.dc.engr.scu.edu", 44282);

    th1.join();

    return 0;
}