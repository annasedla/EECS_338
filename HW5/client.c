// Chris Fietkiewicz. Demonstrates C sockets. Designed to work with server.c.
// Usage: client ipaddress port
// Example: client 127.0.0.1 8000
// Based on socket example from http://www.linuxhowtos.org/C_C++/socket.htm

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

// Helper function to conveniently print to stderr AND exit (terminate)
void error(const char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    // Check for proper number of commandline arguments
    // Expect program name in argv[0], IP address in argv[1], and port # in argv[2]
    // Get socket
    int portno = 8000;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    // Set up for connect()
    struct hostent *server;
    server = gethostbyname("127.0.0.1");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
   
    // Make connection
    if (connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");


      // Get user input and write (send) to server
    char buffer[256];
    bzero(buffer, sizeof(buffer));
    int n = read(sockfd, buffer, sizeof(buffer));
    printf("%s ", buffer);

    // Get user input and write (send) to server
    bzero(buffer, sizeof(buffer));
    fgets(buffer, sizeof(buffer), stdin);
    n = write(sockfd, buffer, sizeof(buffer));

    if (n < 0) 
         error("ERROR writing to socket");

    // Get response from server (must give us a response!)
    bzero(buffer, strlen(buffer));
    n = read(sockfd, buffer, sizeof(buffer));
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n", buffer);
    close(sockfd);
    return 0;
}
