#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define BUFFER_SIZE 1024
int main() {
int sockfd;
struct sockaddr_in server_addr;
char buffer[BUFFER_SIZE] = {0};
// Create a socket
if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
perror("socket creation failed ");
exit(EXIT_FAILURE);
}
// Set up the server address structure
memset(&server_addr, 0, sizeof(server_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(PORT);
// Convert the server IP address from text to binary form
if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
perror("Invalid address / Address not supported");
close(sockfd);
exit(EXIT_FAILURE);
}
// Connect to the server
if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
perror("Connection Failed");
close(sockfd);
exit(EXIT_FAILURE);
}
printf("connected to server\n");
// Send a message to the server
//scanf("%c",&hold);
while (1) {
        // Get user input
        printf("Client: ");
        scanf("%s",buffer);


        // Send message to the server
        send(sockfd, buffer, strlen(buffer), 0);

        // Check if user wants to quit
        if (strcmp(buffer, "quit") == 0) {
            printf("Closing connection\n");
            break;
        }

        // Receive echoed message from the server
        int bytes_read = read(sockfd, buffer, BUFFER_SIZE - 1);
        if (bytes_read <= 0) {
            printf("no data has been sent\n");
            break;
        }

        buffer[bytes_read] = '\0'; // Null-terminate received string
        printf("Server: %s\n", buffer);
    }
// Close the socket
close(sockfd);
return 0;
}
