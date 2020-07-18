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
    int bytesRecvd;
    Client threadClient(hostname_or_ip, portno);

    threadClient.connectToServer();
    if (threadClient.errorFlag) {
        return;
    }

    do {
        std::stringstream buffStream;
        std::string temp;
        
        bytesRecvd = threadClient.receiveFromServer();
        if (threadClient.errorFlag) {
            return;
        }

        if (bytesRecvd > 0) {
            std::cout << threadClient.buffer;

            buffStream << threadClient.buffer;
            getline(buffStream, temp, ',');
            //std::cout << std::stoi(temp) << ", ";
            getline(buffStream, temp, ',');
            //std::cout << temp << std::endl;
        }
    }
    while (bytesRecvd != 0);

    close(threadClient.sockfd);
}

int main(void) {
    thread th1(streamThread, "linux10620.dc.engr.scu.edu", 42217);

    th1.join();

    return 0;
}