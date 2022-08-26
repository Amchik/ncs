#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <assert.h>
#include <signal.h>

const char WRITEBUFF[] = 
  "Hehe, im writting to socket yahoo!\n"
  "By the way, this text can be edited.\n"
  "(hey, pls close connection OwO)\n"
  "";

int main() {
  socklen_t slen;
  struct sockaddr_un addr;
  
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, "/tmp/sock_as_file.socket", sizeof addr.sun_path);

  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  assert(fd > 0);
  assert(
      connect(fd, (struct sockaddr*)&addr, sizeof addr)
      != -1);

  write(fd, WRITEBUFF, sizeof WRITEBUFF);

  return 0;
}

