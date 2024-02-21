#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void) {
  printf("Hello World");
  printf("\nServer Up\n");

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  if (listen(sock, 32) < 0) {
    perror("Error in Listen");
    exit(EXIT_FAILURE);
  }

  printf("%d\n", sock);

  return 0;
}
