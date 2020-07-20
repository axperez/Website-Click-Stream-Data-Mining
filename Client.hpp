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

        Client(std::string host_or_ip, int portnum);
        void connectToServer(void);
        int receiveFromServer(void);
        void error(std::string msg);
};