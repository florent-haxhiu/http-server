#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "sockets.h"

#define BUFFER_SIZE 1024

void init_buffer(char *buffer) {
  for (int i = 0; i < 1024; i++) {
    buffer[i] = 0;
  }
}

int main() {
  // Disable output buffering
  setbuf(stdout, NULL);

  printf("Client connected\n");

  char recvBuffer[1024];

  init_buffer(recvBuffer);

  struct sockets_t sockets = start_server();

  if (sockets.return_code < 0) {
    printf("Error\n");
    return -1;
  }

  int count = read(sockets.client_fd, recvBuffer, sizeof(recvBuffer) - 1);

  if (count == -1 || count == 0) {
    printf("Unable to read request\n");
    close(sockets.server_fd);
    return -1;
  }

  char method[BUFFER_SIZE], path[BUFFER_SIZE], protocol[BUFFER_SIZE];
  char headers[BUFFER_SIZE], headers_key[BUFFER_SIZE], headers_value[BUFFER_SIZE];

  sscanf(recvBuffer, "%s %s %s", method, path, protocol);
  printf("%s", recvBuffer);

  const char *header_start = strchr(recvBuffer, '\r\n');
  sscanf(header_start, "%s: %s", headers_key, headers_value);
  printf("%s %s\n", headers_key, headers_value);
  while (sscanf(header_start, "%s: %s", headers_key, headers_value) == 2) {
    printf("Header = %s=%s\n", headers_key, headers_value);
    header_start = strchr(recvBuffer, '\r\n') + 1;
  }

  char ok[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: ";
  char not_found[] = "HTTP/1.1 404 Not Found\r\n\r\n";

  char *start = strstr(path, "/echo");

  if (strcmp(path, "/") == 0) {
    char ok_2[] = "HTTP/1.1 200 OK\r\n\r\n";
    send(sockets.client_fd, ok_2, strlen(ok_2), 0);
    printf("200");
  } else if (start != NULL) {
    strcpy(&start[0], &start[6]);

    int length = strlen(start);
    char length_of_word[10];

    sprintf(length_of_word, "%d", length);
    strcat(length_of_word, "\r\n\r\n");

    strcat(ok, length_of_word);
    strcat(ok, start);

    printf("%s\n", ok);

    send(sockets.client_fd, ok, strlen(ok), 0);
    printf("200");
  } else {
    send(sockets.client_fd, not_found, strlen(not_found), 0);
    printf("404");
  }

  printf("\nServer now closed\n");

  close(sockets.server_fd);

  return 0;
}
