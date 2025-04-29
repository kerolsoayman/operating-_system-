#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#define PORT 8080
#define BUFFER_SIZE 1024
const int  MAX_CLIENTS =100  ;// Limit to FD_SETSIZE for portability

int main()
{
    int server_fd,udp_fd, new_socket, client_sockets[MAX_CLIENTS];
    struct sockaddr_in address,udp_client_addr;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    int maxfd, i;
    fd_set rset;

    // Initialize client sockets list
    for (i = 0; i < MAX_CLIENTS; i++)
        client_sockets[i] = -1;

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if ((udp_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("UDP socket failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Bind
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
	//bind tcp
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        close(udp_fd);
        exit(EXIT_FAILURE);
    }
	//bind udp
	 if (bind(udp_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        close(udp_fd);
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 10) < 0)
    {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);



    while (1)
    {
        FD_ZERO(&rset);
        FD_SET(server_fd, &rset);
		FD_SET(udp_fd, &rset);
        maxfd = (udp_fd>server_fd)?udp_fd:server_fd;
		//see which is active udp or tcp
        // Add client sockets to the fd_set
        for (i = 0; i < MAX_CLIENTS; i++)
        {
            int sd = client_sockets[i];
            if (sd > 0)
                FD_SET(sd, &rset);
            if (sd > maxfd)
                maxfd = sd;
        }

        // Wait for activity
        select(maxfd + 1, &rset, NULL, NULL, NULL);

        // Check if it's a new connection
        if (FD_ISSET(server_fd, &rset))
        {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                perror("accept failed");
                continue;
            }

            printf("New client connected: socket %d\n", new_socket);

            // Add new socket to array
            for (i = 0; i < MAX_CLIENTS; i++)
            {
                if (client_sockets[i] == -1)
                {
                    client_sockets[i] = new_socket;
                    break;
                }
            }

            if (i == MAX_CLIENTS)
            {
                printf("Too many clients\n");
                close(new_socket);
            }
        }
		else if(FD_ISSET(udp_fd, &rset))
		{
            socklen_t udp_client_len = sizeof(udp_client_addr);
            int bytes_received = recvfrom(udp_fd, buffer, BUFFER_SIZE - 1, 0,
                            (struct sockaddr *)&udp_client_addr, &udp_client_len);

            if (bytes_received < 0)
            {
                perror("recvfrom failed");
            }
            else
            {
                buffer[bytes_received] = '\0';
                printf("Udp client: %s\n" , buffer);

                // Echo back to the same client
                sendto(udp_fd, buffer, bytes_received, 0,
                       (struct sockaddr *)&udp_client_addr, sizeof(udp_client_addr));
            }


		}

        // Check all clients for incoming data
        for (i = 0; i < MAX_CLIENTS; i++)
        {
            int sd = client_sockets[i];
            if (sd > 0 && FD_ISSET(sd, &rset))
            {
                int bytes_read = read(sd, buffer, BUFFER_SIZE - 1);

                if (bytes_read <= 0)
                {
                    printf("TCP Client %d didn't send data \n", sd);
                    close(sd);
                    client_sockets[i] = -1;
                }
                else
                {
                    buffer[bytes_read] = '\0';
                    printf("TCP Client %d sent: %s\n", sd, buffer);

                    if (strcmp(buffer, "quit") == 0)
                    {
                        printf("TCP Client %d requested to close connection.\n", sd);
                        close(sd);
                        client_sockets[i] = -1;
                    }
                    else
                    {
                        send(sd, buffer, bytes_read, 0);  // Echo back
                    }
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
