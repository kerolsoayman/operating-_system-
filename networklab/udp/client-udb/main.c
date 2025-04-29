#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int sockfd, maxfdp1;
    fd_set rset;
    struct sockaddr_in server_addr;
    socklen_t addr_len;
    char buffer[BUFFER_SIZE] = {0};

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IP address
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0)
    {
        perror("Invalid address / Address not supported");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP client ready to send/receive\n");

    addr_len = sizeof(server_addr);

    while (1)
    {
        FD_ZERO(&rset);
        FD_SET(fileno(stdin), &rset);
        FD_SET(sockfd, &rset);

        maxfdp1 = (fileno(stdin) > sockfd ? fileno(stdin) : sockfd) + 1;

        select(maxfdp1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset))
        {
            int bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                                          (struct sockaddr *)&server_addr, &addr_len);
            if (bytes_received < 0)
            {
                perror("recvfrom failed");
                break;
            }
            buffer[bytes_received] = '\0'; // Null-terminate
            printf("Server: %s\n", buffer);
        }

        if (FD_ISSET(fileno(stdin), &rset))
        {
            

            if (strcmp(buffer, "quit") == 0)
            {
                printf("Closing UDP client\n");
                break;
            }

            sendto(sockfd, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&server_addr, sizeof(server_addr));
        }
    }

    close(sockfd);
    return 0;
}
