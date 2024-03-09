#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory>
#include <cstring>
#include "booking.h"


const char* const OPTARGS = "hvp:i:";

const char* const  HLPMSG = "Usage: kino [ OPTIONS ]\n"\
                            "-h this help message\n"\
                            "-v be verbose\n"\
                            "-p PORT - to listen for incoming connections (default 42024)\n"\
                            "-i IPv4 - to listen (default 127.0.0.1)\n" ;                                                       

const unsigned KDefaultTCPPort = 42024;
const char* const KDefaultTCPAddress = "localhost";

int main(int argc, char **argv)
{
    int  opt;
    bool verbose = false;
    int  port = KDefaultTCPPort;
    std::string addr {KDefaultTCPAddress};

    /// handle CLI
    ///no question Boost.Program_options is much more convenient but imho it is not reasonable to add dependency to boost just for this purpose
    ///while good old schhol getopt is always here
    while ((opt = getopt(argc, argv, OPTARGS)) != -1) {
        switch (opt) {
        case 'v': verbose=true; break;
        case 'p': port = atoi(optarg); break;
        case 'i': addr = (const char* const)optarg; break;
        case 'h': //fallthry
        default: printf(HLPMSG); return 0;
        };
    };

    
    /*
      daemonize, detach from tty
    */
    pid_t cpid = fork();

    if (cpid == -1) {
      perror("Failed to daemonize, Bye"); 
      exit(EXIT_FAILURE); 
    }

    if (cpid > 0) 
        exit(EXIT_SUCCESS);


    int fd = open("/dev/tty", O_RDWR);
    ioctl(fd, TIOCNOTTY, NULL);
    setpgid(getpid(), 0);

    /*
      then start TCP server
      NB: yes this is a quick & dirty solution and for production grade code a kind of off-the-shelf one (like ngnix or httpd) shall be choosen 
      or even better, RESTFul server like Swagger (OpenAPI)  
    */

   	int server_fd, new_socket;
    long valread;
    struct sockaddr_in address{0};
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Unable to open socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    std::memset(address.sin_zero, 0, sizeof address.sin_zero);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("Unable to  listen");
        exit(EXIT_FAILURE);
    }

      for (;;) {

        if (verbose)
            printf("\n Waiting for connections\n\n");

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) { 
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        ///start new thread and handle request there  
        //https://stackoverflow.com/questions/3060950/how-to-get-ip-address-from-sock-structure-in-c
        struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&address;
        struct in_addr ipAddr = pV4Addr->sin_addr;
        char str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &ipAddr, str, INET_ADDRSTRLEN);

        if (verbose)
          printf("Client IP: %s", str);

        char buffer[1500] = { 0 };  ///circa MTU for the sake of simplicity
        valread = read(new_socket, buffer, 1500);

        if (verbose)
          printf("\n%s \n ", buffer);
    }
}