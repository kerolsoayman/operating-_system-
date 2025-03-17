#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define BUFFER_SIZE 1024
int main() {
int server_fd, new_socket;
struct sockaddr_in address;
int addrlen = sizeof(address);
char buffer[BUFFER_SIZE] = {0};
char *response;
int i=0;
// Create socket
if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
perror("socket failed");
exit(EXIT_FAILURE);
}
// Bind socket to address and port
address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons(PORT);
if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
perror("bind failed");
close(server_fd);
exit(EXIT_FAILURE);
}
// Listen for incoming connections
if (listen(server_fd, 3) < 0) {
perror("listen failed");
close(server_fd);
exit(EXIT_FAILURE);
}
printf("Server listening on port %d\n", PORT);
while (1) {
        // Accept a new connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        printf("Client connected\n");

        while (1) {
            memset(buffer, 0, BUFFER_SIZE); // Clear buffer
            int bytes_read = read(new_socket, buffer, BUFFER_SIZE - 1);

            if (bytes_read <= 0) {
                printf("Buffer is empty");
                break;  // Exit loop if read fails or client disconnects
            }

            buffer[bytes_read] = '\0'; // Null-terminate received string
            printf("Received: %s\n", buffer);

            // Check if client sent "quit"
            if (strcmp(buffer, "quit") == 0) {
                printf("Client requested to close connection.\n");
                break;  // Break out of the loop and close connection
            }

            // Echo message back to client
            send(new_socket, buffer, bytes_read, 0);
        }

        close(new_socket); // Close client connection
    }
// Close the server socket
close(server_fd);
return 0;
}
