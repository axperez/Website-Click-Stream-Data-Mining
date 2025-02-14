// =========================================================
//      Client.cpp
// 
// ---------------------------------------------------------
// Author:      Axel Perez
// Date:        7/19/20
// =========================================================

#include "Client.hpp"
using namespace std;

// Client Class Constructor:
//      Creates socket, retrieves and saves host info.
Client::Client(string host_or_ip, int portnum) {
    portno = portnum;
    hostname = host_or_ip;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening socket\n");
        return;
    }

    server = gethostbyname(host_or_ip.c_str());
    if (server == NULL) {
        error("Error no such host: " + host_or_ip);
        return;
    }

    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(portno);
    ip = (char *)server->h_addr;
}

// Client Connect to Server Member Function:
//      Attempts to connect to socket.
void Client::connectToServer(void) {
    if(connect(sockfd, (const sockaddr*) &server_address, sizeof(server_address)) < 0) {
        error("Error connecting to " + hostname + " on port " + to_string(portno));
    }
}

// Client Receive from Server Member Function:
//      Receive data from server into buffer and return 
//      the number of bytes received.
int Client::receiveFromServer(void) {
    int bytesRecvd;

    bzero(buffer, BUFF_SIZE);
    bytesRecvd = recv(sockfd, buffer, BUFF_SIZE-1, 0);
    if (bytesRecvd < 0) {
        error("Error receiving data for host " + hostname + " on port " + to_string(portno));
    }

    return bytesRecvd;
}

// Client Error Member Function:
//      Closes socket, prints error message, and sets object's error flag.
void Client::error(string msg) {
    close(sockfd);
    perror(msg.c_str());
    errorFlag = true;
}