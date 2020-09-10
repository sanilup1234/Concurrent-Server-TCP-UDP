#include<stdio.h>

#include<sys/types.h>//socket

#include<sys/socket.h>//socket

#include<string.h>//memset

#include<stdlib.h>//sizeof

#include<netinet/in.h>//INADDR_ANY

#include <unistd.h>

#include<bits/stdc++.h>

#include <sys/wait.h>

using namespace std;

void udp_handler(int port_number) {
    struct sockaddr_in server_address; //sever address
    struct sockaddr_in client_address; // cliet address

    int connection_fd; // connection descriptor

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); // creting UDP socket
    if (sockfd == 0) {
        printf("socket creation failed...\n"); //if socket creation failed->exit
        exit(0);
    }
    // assigning IP and port number to the UDP sever
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY for making local host as server  
    server_address.sin_port = htons(port_number); // settint port number for udp server

    if (bind(sockfd, (struct sockaddr * ) & server_address, sizeof(server_address)) != 0) // binding the socket
    {
        printf("bind  failed...\n"); // if binding is failed
        exit(0);
    }
    socklen_t client_address_length;
    client_address_length = sizeof(client_address);
    int length;
    char message[500]; //for receving short msg from client 

    length = recvfrom(sockfd, (char * ) message, 500, 0, (struct sockaddr * ) & client_address, & client_address_length);
    if (length <= 0)
        cout << "error";
    else {
        message[length - 1] = '\0';
    }
    cout << "MESSAGE RECEIVED BY UDP SERVER FROM UDP CLIENT: " << message << "...............(type3 message)" << endl << endl;

    close(sockfd);

}

int main(int argc, char * argv[]) {
    //Checking format validity
    if (argc != 2) {
        cout << "wrong format ( Format must be ./server port_number)";
        exit(1);
    }
    int port_number = atoi(argv[1]);
    //Checking port number validity
    if (port_number < 1024 || port_number > 65535) {
        cout << "invalid port number (Port number must lie between 1024-65535)";
        exit(1);
    }

    int pid;
    socklen_t client_address_length;
    struct sockaddr_in server_address; //server receive on this address
    struct sockaddr_in client_address; //server sends to client on this address
    int connection_fd;
    char message[256];
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("TCP socket creation failed...\n");
        exit(0);
    }
    // assigning IP and port number to the TCP sever
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port_number);

    if (bind(sockfd, (struct sockaddr * ) & server_address, sizeof(server_address)) != 0) //binding the socket to ip and port number 
    {
        printf("bind  failed...\n"); //when binding failed
        exit(0);
    }
    // server is listening s
    if ((listen(sockfd, 7)) != 0) //maximum 7 coonection can be in pending queue
    {
        printf("Listen failed...\n");
        exit(0);
    }
    while (true) {
        cout << "\n**************************       SERVER IS WAITING           **************************************\n";
        client_address_length = sizeof(client_address);
        connection_fd = accept(sockfd, (struct sockaddr * ) & client_address, & client_address_length); // accepting connections 
        pid = fork(); // child process for handling multiple clients concurrently

        if (pid == 0) {

            srand(time(0));
            int generated_port = 1023 + rand() % 65353; //generate the port number to be sent to the TCP client

            for (;;) {
                if (recv(connection_fd, message, 256, 0) == 0) // recieveing the request message
                {

                    close(connection_fd);

                    break;
                }

                cout << "\nMESSAGE SENT BY THE CLIENT :" << message << ".......................(type1 message)" << endl << endl;

                break;

            }

            int port_to_send = htonl(generated_port);
            cout << "\nPORT NO. SENT TO CLIENT :" << generated_port << "...............................(type2 message)" << endl << endl;

            write(connection_fd, & port_to_send, sizeof(port_to_send)); //sending the generated port number to the client
            close(connection_fd); //close the TCP connection
            udp_handler(generated_port); //for UDP communication

        } else {
            close(connection_fd);

            break;

        }

    }

    wait(NULL); //wait till child processes are executing
    close(sockfd); //close socket
    return 0;
}
