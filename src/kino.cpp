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
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include "../test/testdata.h"


const char* const OPTARGS = "dhvp:i:";

const char* const  HLPMSG = "Usage: kino [ OPTIONS ]\n"\
                            "-h this help message\n"\
                            "-d daemonize\n"\
                            "-v be less tolkative\n"\
                            "-p PORT - to listen for incoming connections (default 42024)\n"\
                            "-i IPv4 - to listen (default 127.0.0.1)\n" ;                                                       

const unsigned KDefaultTCPPort = 42024;
const char* const KDefaultTCPAddress = "localhost";

char* parse(char line[], const char symbol[]);
int send_message(int fd, char image_path[], char head[]);

char http_header[25] = "HTTP/1.1 200 Ok\r\n";

int main(int argc, char **argv)
{
    int  opt;
    bool verbose = true;
    bool daemonize = false; //do not daemonize running in docker
    int  port = KDefaultTCPPort;
    std::string addr {KDefaultTCPAddress};

    /// handle CLI
    ///no question Boost.Program_options is much more convenient but imho it is not reasonable to add dependency to boost just for this purpose
    ///whilest good old schhol getopt is always here
    while ((opt = getopt(argc, argv, OPTARGS)) != -1) {
        switch (opt) {
        case 'd': daemonize=true; break;
        case 'v': verbose=true; break;
        case 'p': port = atoi(optarg); break;
        case 'i': addr = (const char* const)optarg; break;
        case 'h': //fallthry
        default: printf(HLPMSG); return 0;
        };
    };

    booking::Booking::Initialize(testdata::JSON);
    
    /*
      daemonize, detach from tty
    */
    if (daemonize) {
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
    }
    /*
      then start TCP server
      NB: yes this is a quick & dirty solution and for production grade code a kind of off-the-shelf one (like ngnix or httpd) shall be choosen 
      or even better, RESTFul server like Swagger (OpenAPI)  
    */

   	int server_fd;
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

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("Unable to  listen");
        exit(EXIT_FAILURE);
    }

    ////TODO: this is wery-wery naive single shot solution. actually thread pool shall be used here 
   	while (1)
    {
        printf("\n Waiting for new connection \n");
        int new_socket;

        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        char buffer[1500] = { 0 };
        valread = read(new_socket, buffer, 1500);

        printf("\n%s \n ", buffer);

        char *parse_string = parse(buffer, " ");  //Try to get the path which the client ask for
        printf("Client ask for path: %s\n", parse_string);
        char *copy_head = (char *)malloc(strlen(http_header) + 200);
        strcpy(copy_head, http_header);

        std::unique_ptr<booking::BookingClient> b = booking::BookingClient::New();

        if (strcmp(parse_string, "/theaters") == 0) 
        {
            auto cl = b->GetTheaterList();

            rapidjson::Document document;
            document.SetArray();

            rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
            
            for (int i = 0; i < cl.size(); i++) {
                rapidjson::Value value;
                value.SetString(cl.at(i).iName.c_str(), cl.at(i).iName.length(), allocator);
                document.PushBack(value, allocator);
            }

            rapidjson::StringBuffer strbuf;
            rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
            document.Accept(writer);

            std::string json{strbuf.GetString()};

            strcat(copy_head, "Content-Type: application/json\r\n\r\n");
            write(new_socket, copy_head, strlen(copy_head));
            write(new_socket, json.c_str(), json.length());
            close(new_socket);
        } 
        else if (strcmp(parse_string, "/movies") == 0)
        {
            auto cl = b->GetMovieList();
            rapidjson::Document document;
            document.SetArray();

            rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
            
            for (int i = 0; i < cl.size(); i++) {
                rapidjson::Value value;
                value.SetString(cl.at(i).iName.c_str(), cl.at(i).iName.length(), allocator);
                document.PushBack(value, allocator);
            }

            rapidjson::StringBuffer strbuf;
            rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
            document.Accept(writer);

            std::string json{strbuf.GetString()};

            strcat(copy_head, "Content-Type: application/json\r\n\r\n");
            write(new_socket, copy_head, strlen(copy_head));
            write(new_socket, json.c_str(), json.length());
            close(new_socket);
        }
	  }
	  return 0;
}

char* parse(char line[], const char symbol[])
{
    char *message;
    char *token = strtok(line, symbol);
    int current = 0;

    while (token != NULL) 
    {
        token = strtok(NULL, " ");

        if (current == 0) {
            message = token;
            return message;
        }

    current = current + 1;
    }
    return message;
}


