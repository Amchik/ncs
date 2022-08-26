#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <assert.h>
#include <signal.h>

int main() {
  socklen_t slen;
  struct sockaddr_un addr;
  
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, "/tmp/sock_as_file.socket", sizeof addr.sun_path);

  unlink("/tmp/sock_as_file.socket");

  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  assert(fd > 0);
  assert(
      bind(fd, (struct sockaddr*)&addr, sizeof addr)
      != -1);

  assert(
      listen(fd, 1)
      != -1);

  slen = 0;

  int cfd = accept(fd, (struct sockaddr*)&addr, &slen);
  assert(cfd > 0);

  printf("Got client FD: \033[34m%d\033[0m\n", cfd);

  /* We do a little trolling */
  FILE* fp = fdopen(cfd, "a+");
  char *line;
  size_t lsize = 0, nread;
  while ((nread = getline(&line, &lsize, fp)) != -1) {
    printf("\033[34m%.3lu bytes\033[0m | %s", nread, line);
    if (line[nread - 1] != '\n') {
      printf("\n");
    }
  }
  printf("Terminated due \033[34mgetline()\033[0m returned \033[34m%ld\033[0m\n", nread);

  return 0;
}

