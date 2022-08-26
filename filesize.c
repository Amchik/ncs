#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  int fd = open(argv[1], 0);
  if (fd < 0) {
    perror("open()");
    return 1;
  }

  off_t fdsize = lseek(fd, 0, SEEK_END);
  printf("File size using \033[34mlseek()\033[0m: %lu\n", fdsize);

  close(fd);

  FILE *fp = fopen(argv[1], "r");
  if (!fp) {
    perror("fopen()");
    return 1;
  }

  fseek(fp, 0, SEEK_END); /* NOTE: return value of this call equals fpsize value */
  long fpsize = ftell(fp);
  printf("File size using \033[34mftell()\033[0m: %ld\n", fdsize);

  return fclose(fp);
}

