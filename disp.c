#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/syscall.h>
#include <signal.h>

#define inline __attribute__((always_inline))

inline void start_reading();
inline void stop_reading();

__attribute__((destructor))
void destructor() {
  stop_reading();
  exit(0);
}

int main(int argc, char **argv) {
  off_t len;
  off_t offset;
  int c, fd;
  struct termios info;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return(1);
  }

  fd = open(argv[1], 0 /* just read */);
  if (fd < 0) {
    perror("Failed to open file");
    return(2);
  }

  len = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);

  /* clear */
  syscall(SYS_write, STDOUT_FILENO, "\033[H\033[2J\033[3J", 11);

  offset = 0;
  sendfile(STDOUT_FILENO, fd, &offset, len);

  signal(SIGINT, destructor);
  signal(SIGTERM, destructor);
  start_reading();
  while (1) {
    c = getchar();
    if (c == 27 || c == 'q') {
      stop_reading();
      return(0);
    }
  }
}

void start_reading() {
  struct termios info;

  fcntl(0, F_SETFL, O_NONBLOCK);
  tcgetattr(0, &info);
  info.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
  info.c_lflag &= ~ICANON;
  info.c_cc[VMIN] = 1;
  info.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &info);

  /* hide cursor */
  printf("\e[?25l");
}
void stop_reading() {
  struct termios info;

  tcgetattr(0, &info);
  info.c_lflag |= (ECHO | ECHOE | ECHOK | ECHONL);
  info.c_lflag |= ICANON;
  tcsetattr(0, TCSANOW, &info);

  /* show cursor */
  printf("\e[?25h");
}

