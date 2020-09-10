#include<stdio.h>

#include<sys/types.h>// for socket

#include<sys/socket.h>//for msocket

#include<string.h>//for memset

#include<stdlib.h>//for sizeof

#include<netinet/in.h>//for INADDR_ANY

#include <unistd.h>

#include<bits/stdc++.h>

#include <sys/wait.h>

#include <arpa/inet.h>

#define WIN32_LEAN_AND_MEAN

using namespace std;

void udp_client(int port_number, char * ip_address) {
    struct sockaddr_in server_address;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); //UDP socket treation
    if (sockfd < 0) {
        cout << "\nUDP socket cration failure\n"; //when socket creation failed-> exit
        return;
    }
    // assigning IP and port to the sever address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip_address);
    server_address.sin_port = htons(port_number);

    char message[100]; //message of type 3
    cout << "\nEnter message to send to server:\n";
    fgets(message, 100, stdin);

    // sending the message to the server
    sendto(sockfd, (const char * ) message, strlen(message) + 1, 0, (const struct sockaddr * ) & server_address, sizeof(server_address));
    close(sockfd); // closing the socket

}

int main(int argc, char ** argv) {
    //check for input Format validity
    if (argc != 3) {
        cout << "Wrong Format (./client server_address port_number)";
        exit(0);
    }

    char * ip_address = argv[1];
    // checking ip address validity
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ip_address, & (sa.sin_addr));
    if (result == 0) {
        cout << "Invalid Ip Address";
        exit(0);
    }

    int port_number = atoi(argv[2]);

    // checking port number validity
    if (port_number < 1024 || port_number > 65535) {
        cout << "Invalid port number";
        exit(1);
    }

    struct sockaddr_in server_address; //for server address

    char message[100] = " UDP port request message "; // message of type1 

    int sockfd = socket(AF_INET, SOCK_STREAM, 0); //creating TCP client socket
    if (sockfd == -1) {
        printf("TCP socket creation failed...\n"); //when socket creation is failed
        exit(0);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip_address); //setting server ip address
    server_address.sin_port = htons(port_number); // setting port number to which the connection is to be maid

    if ((connect(sockfd, (struct sockaddr * ) & server_address, sizeof(server_address))) != 0) //trying to connect to ther server
    {
        cout << "connection failed";
        exit(1); //exit if connection failed
    }

    send(sockfd, message, strlen(message) + 1, 0); // sending  the message to the client
    int received_port = -1;
    read(sockfd, & received_port, sizeof(received_port)); // receiving the port number from server
    close(sockfd); //close the socket
    printf("Receive port number from  server(type2): %d\n", ntohl(received_port));
    udp_client(ntohl(received_port), argv[1]);

    return 0;
}
