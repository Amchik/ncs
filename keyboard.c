/* vim: ft=c nowrap
 * license: The Unlicense (unlicense.org)
 * original author: Amchik <github.com/Amchik>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <termios.h>
#include <fcntl.h>

#define $(str) ((Component){ .cnt = str, .hi = false })

typedef struct {
  char cnt;
  bool hi;
} Component;
struct Line {
  Component c[16];
} LINES[] = {
  { .c = { $('q'), $('w'), $('e'), $('r'), $('t'), $('y'), $('u'), $('i'), $('o'), $('p'), $('['), $(']'), $(0) } },
  { .c = { $('a'), $('s'), $('d'), $('f'), $('g'), $('h'), $('j'), $('k'), $('l'), $(';'), $('\''), $('\\'), $(0) } },
  { .c = { $('z'), $('x'), $('c'), $('v'), $('b'), $('n'), $('m'), $(','), $('.'), $('/'), $(0) } },
  { .c = { $(0) } }
};

int main() {
  size_t i, j;
  int c;
  struct termios info;

  fcntl(0, F_SETFL, O_NONBLOCK);
  tcgetattr(0, &info);
  info.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
  info.c_lflag &= ~ICANON;
  info.c_lflag &= ~(VSTOP | VSUSP | VQUIT);
  info.c_cc[VMIN] = 1;
  info.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &info);

  puts("Press \033[34m<ESC>\033[0m to exit");
  goto print;

  while ((c = getchar()) != 27) {
    if (c == 0 || c == -1)
      continue;

    for (i = 0; LINES[i].c[0].cnt != 0; i += 1) {
      for (j = 0; LINES[i].c[j].cnt != 0; j += 1) {
        if (LINES[i].c[j].hi)
          LINES[i].c[j].hi = false;
        if (LINES[i].c[j].cnt == c)
          LINES[i].c[j].hi = true;
      }
    }

print:
    for (i = 0; LINES[i].c[0].cnt != 0; i += 1) {
      for (j = 0; j < i; j += 1)
        printf("   ");
      for (j = 0; LINES[i].c[j].cnt != 0; j += 1) {
        if (LINES[i].c[j].hi)
          printf("\033[0;7m");
        printf("[ %c ]\033[0m ", LINES[i].c[j].cnt);
      }
      printf("\n\n");
    }
    printf("\033[%luA", i * 2);
  }
  tcgetattr(0, &info);
  info.c_lflag |= (ECHO | ECHOE | ECHOK | ECHONL);
  info.c_lflag |= ICANON;
  info.c_lflag |= (VSTOP | VSUSP | VQUIT);
  tcsetattr(0, TCSANOW, &info);

  return 0;
}

