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

// constants
const unsigned int BUFF_SIZE = 512;

// Function prototypes
void error(std::string msg);

// Class definitions
class Client {
    public:
        int sockfd, portno, bytesRecvd = -1;
        struct sockaddr_in server_address;
        struct hostent *server;
        char buffer[BUFF_SIZE];

        Client(std::string host_or_ip, int portnum) {
            portno = portnum;
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

            if(connect(sockfd, (const sockaddr*) &server_address, sizeof(server_address)) < 0) {
                error("Error connecting to " + host_or_ip + " on port " + std::to_string(portno));
            }
            while (bytesRecvd != 0) {
                bzero(buffer, BUFF_SIZE);
                bytesRecvd = recv(sockfd, buffer, BUFF_SIZE-1, 0);
                
                if (bytesRecvd < 0) {
                    error("Error receiving data for host " + host_or_ip + " on port " + std::to_string(portno));
                }
                else if (bytesRecvd > 0)
                    std::cout << bytesRecvd << " received: " << buffer << std::endl;
            }
            close(sockfd);
        }
};

void error(std::string msg) {
    perror(msg.c_str());
    exit(0);
}