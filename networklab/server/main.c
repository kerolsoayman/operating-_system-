#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<signal.h>
#include<pthread.h>
#define PORT 8080
#define BUFFER_SIZE 1024
typedef    void    Sigfunc(int);
Sigfunc *signal (int signo, Sigfunc *func)
  {
      struct sigaction act, oact;
      act.sa_handler = func;
      sigemptyset (&act.sa_mask);
      act.sa_flags = 0;
      if (signo == SIGALRM) {
  #ifdef  SA_INTERRUPT
          act.sa_flags |= SA_INTERRUPT;     /* SunOS 4.x */
  #endif
      } else {
  #ifdef  SA_RESTART
          act.sa_flags |= SA_RESTART; /* SVR4, 4.4BSD */
  #endif
      }
      if (sigaction (signo, &act, &oact) < 0)
          return (SIG_ERR);
      return (oact.sa_handler);
  }
  void sig_chld(int signo)
  {
      pid_t   pid;
      int     stat;
      while((pid = waitpid (-1, &stat, WNOHANG))>0)
      printf("child %d termenatied" ,pid);
      return;
  }

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

int child=0;
int cn;
signal(SIGCHLD,sig_chld);

while (1) {
        // Accept a new connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        child++;
            if((fork())==0)
                {
                close(server_fd);
                cn=child;


        printf("Client %d connected \n",cn);

        while (1) {
            memset(buffer, 0, BUFFER_SIZE); // Clear buffer
            int bytes_read = read(new_socket, buffer, BUFFER_SIZE - 1);

            if (bytes_read <= 0) {
                printf("Buffer is empty");
                break;  // Exit loop if read fails or client disconnects
            }

            buffer[bytes_read] = '\0'; // Null-terminate received string
            printf("client %d sent: %s \n", cn,buffer);

            // Check if client sent "quit"
            if (strcmp(buffer, "quit") == 0) {
                printf("Client %d requested to close connection.\n",cn);
                break;  // Break out of the loop and close connection
            }

            // Echo message back to client
            send(new_socket, buffer, bytes_read, 0);
        }


        close(new_socket);
        exit(0);
        } // Close client connection
    }

// Close the server socket
close(server_fd);
return 0;
}
