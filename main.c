#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void die(const char *message) {
  fprintf(stderr, "Error: %s\n", message);
  exit(EXIT_FAILURE);
}

int main(void) {
  printf("Server Up\n");

  int new_socket;
  int val = 1;

  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    die("socket failed");
  }

  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &val, sizeof(val));

  // bind, this will deal with IPv4 addresses
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(8080);

  if (bind(fd, (struct sockaddr *)&addr, addrlen) < 0) {
    die("bind()");
  }

  if (listen(fd, 3) < 0) {
    die("Error in Listen");
  }

  new_socket = accept(fd, (struct sockaddr *)&addr, &addrlen);

  if (new_socket == -1) {
    die("accept()");
  }

  char buffer[1024];
  char *response = "HTTP/1.1 200 OK\r\n\r\n";

  int r = read(new_socket, buffer, 1024 - 1); // minus one for the null terminator

  printf("%s\n", buffer);
  send(new_socket, response, strlen(response), 0);

  close(fd);

  return 0;
}
