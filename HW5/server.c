// Chris Fietkiewicz. Demonstrates C sockets. Designed to work with client.c.
// Usage: server port
// Example: server 8000
// Based on socket example from http://www.linuxhowtos.org/C_C++/socket.htm

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

// Helper function to conveniently print to stderr AND exit (terminate)
void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
     // Check for proper number of commandline arguments
     // Expect program name in argv[0], port # in argv[1]
     // Setup phase
     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

    printf("Listening for client...\n");

     struct sockaddr_in serv_addr; // Server address struct
     bzero((char *) &serv_addr, sizeof(serv_addr));
     int portno = 8000; // Port number is commandline argument
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
          error("ERROR on binding");
     listen(sockfd, 5);

     // Service phase
     struct sockaddr_in cli_addr;
     socklen_t clilen = sizeof(cli_addr); // Address struct length
     int newsockfd = accept(sockfd, 
                     (struct sockaddr *) &cli_addr, 
                     &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");

     char buffer[256];
     bzero(buffer, sizeof(buffer));
     sprintf(buffer,"How many steps does it take for a client to change a light bulb?");
     write(newsockfd, buffer, sizeof(buffer));
     bzero(buffer, sizeof(buffer));

     int n = read(newsockfd, buffer, sizeof(buffer));

    if (n < 0) 
          error("ERROR reading from socket");


     //HOMEWORK 1 
     int result = atoi(buffer);
     printf("Result: %d\n", result);
    
    if (result == 3){
        printf("Client was correct.\n");
        sprintf(buffer, "Correct! The 3 steps are: create a socket, connect to it, and send a new bulb.");

    } else {
        printf("Client was NOT correct.\n");
        sprintf(buffer, "Nope! The 3 steps are: create a socket, connect to it, and send a new bulb.");
    }

     n = write(newsockfd, buffer, sizeof(buffer));
     if (n < 0) 
          error("ERROR writing to socket");
     close(newsockfd);
     close(sockfd);
     return 0; 
}
