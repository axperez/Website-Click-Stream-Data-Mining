// ============================================================
//      client.hpp
// ------------------------------------------------------------
// Author :     Axel Perez
// Date :       7/12/20
// ============================================================

// include files
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>

// constants
const unsigned int BUFF_SIZE = 4096;

// Class definitions
class Client {
    public:
        int sockfd, portno;
        struct sockaddr_in server_address;
        struct hostent *server;
        std::string hostname, ip;
        char buffer[BUFF_SIZE];
        bool errorFlag = false;

        Client(std::string host_or_ip, int portnum) {
            portno = portnum;
            hostname = host_or_ip;
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) {
                error("Error opening socket\n");
            }

            server = gethostbyname(host_or_ip.c_str());
            if (server == NULL) {
                error("Error no such host: " + host_or_ip);
            }

            bzero((char *) &server_address, sizeof(server_address));
            server_address.sin_family = AF_INET;
            bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
            server_address.sin_port = htons(portno);
            ip = (char *)server->h_addr;
        }

        void connectToServer(void) {
            if(connect(sockfd, (const sockaddr*) &server_address, sizeof(server_address)) < 0) {
                error("Error connecting to " + hostname + " on port " + std::to_string(portno));
            }
        }

        int receiveFromServer(void) {
            int bytesRecvd;

            bzero(buffer, BUFF_SIZE);
            bytesRecvd = recv(sockfd, buffer, BUFF_SIZE-1, 0);
            if (bytesRecvd < 0) {
                error("Error receiving data for host " + hostname + " on port " + std::to_string(portno));
            }

            return bytesRecvd;
        }

        void error(std::string msg) {
            close(sockfd);
            perror(msg.c_str());
            errorFlag = true;
        }
};