/* vim: ft=c nowrap
 * license: The Unlicense (unlicense.org)
 * original author: Amchik <github.com/Amchik>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <termios.h>
#include <fcntl.h>

#define $(str) ((Component){ .cnt = str, .hi = false })
#define BACKSPACE "BACKSPACE"
#define SPACE "         SPACE         "

typedef struct {
  char *cnt;
  bool hi;
} Component;
struct Line {
  int off;
  Component c[16];
} LINES[] = {
  { .off = 0, .c = { $("1"), $("2"), $("3"), $("4"), $("5"), $("6"), $("7"), $("8"), $("9"), $("0"), $("-"), $("="), $("BACKSPACE"), $(0) } },
  { .off = 0, .c = { $("q"), $("w"), $("e"), $("r"), $("t"), $("y"), $("u"), $("i"), $("o"), $("p"), $("["), $("]"), $(0) } },
  { .off = 3, .c = { $("a"), $("s"), $("d"), $("f"), $("g"), $("h"), $("j"), $("k"), $("l"), $(";"), $("\""), $("\\"), $(0) } },
  { .off = 6, .c = { $("z"), $("x"), $("c"), $("v"), $("b"), $("n"), $("m"), $(","), $("."), $("/"), $(0) } },
  { .off = 20, .c = { $(SPACE) } },
  { .off = 0, .c = { $(0) } }
};

int main() {
  size_t i, j, stext_pos, timer;
  int c;
  struct termios info;
  char stext[32];

  fcntl(0, F_SETFL, O_NONBLOCK);
  tcgetattr(0, &info);
  info.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
  info.c_lflag &= ~ICANON;
  info.c_lflag &= ~(VSTOP | VSUSP | VQUIT);
  info.c_cc[VMIN] = 1;
  info.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &info);

  stext_pos = 0;
  timer = 0;
  memset(stext, ' ', sizeof(stext));
  stext[sizeof(stext) - 1] = 0;

  puts("Press \033[34m<ESC>\033[0m to exit\033[?25l");
  goto print;

  while ((c = getchar()) != 27) {
    bool validchar;

    timer += 1;

    if (timer > 200000) {
      for (i = 0; LINES[i].c[0].cnt != 0; i += 1) {
        for (j = 0; LINES[i].c[j].cnt != 0; j += 1) {
          if (LINES[i].c[j].hi)
            LINES[i].c[j].hi = false;
        }
      }
      timer = 0;
      if (c == 0 || c == -1)
        goto print;
    } else if (c == 0 || c == -1)
      continue;

    validchar = false;
    for (i = 0; LINES[i].c[0].cnt != 0; i += 1) {
      for (j = 0; LINES[i].c[j].cnt != 0; j += 1) {
        if (LINES[i].c[j].hi)
          LINES[i].c[j].hi = false;
        if (   (LINES[i].c[j].cnt[0] == c && LINES[i].c[j].cnt[1] == 0)
            || (c == ' '  && !strcmp(LINES[i].c[j].cnt, SPACE))
            || (c == 0x7f && !strcmp(LINES[i].c[j].cnt, BACKSPACE))) {
          timer = 0;
          validchar = true;
          LINES[i].c[j].hi = true;
        }
      }
    }
    if (validchar) {
      if (stext_pos == sizeof(stext)) {
        size_t p;

        stext_pos -= 1;
        for (p = 0; p < sizeof(stext); p += 1) {
          stext[p] = stext[p + 1];
        }
      }

      if (c == 0x7f) {
        if (stext_pos > 0) {
          stext_pos -= 1;
          stext[stext_pos] = ' ';
        }
      } else {
        stext[stext_pos] = c;
        stext[stext_pos + 1] = ' ';
        stext_pos += 1;
      }
    }

print:
    printf("                 [ %.31s ]\n\n", stext);
    for (i = 0; LINES[i].c[0].cnt != 0; i += 1) {
      for (j = 0; j < LINES[i].off; j += 1)
        printf(" ");
      for (j = 0; LINES[i].c[j].cnt != 0; j += 1) {
        if (LINES[i].c[j].hi)
          printf("\033[0;7m");
        printf("[ %s ]\033[0m ", LINES[i].c[j].cnt);
      }
      printf("\n\n");
    }
    printf("\033[%luA", i * 2 + 2);
  }
  tcgetattr(0, &info);
  info.c_lflag |= (ECHO | ECHOE | ECHOK | ECHONL);
  info.c_lflag |= ICANON;
  info.c_lflag |= (VSTOP | VSUSP | VQUIT);
  tcsetattr(0, TCSANOW, &info);

  printf("\e[?25h");

  return 0;
}

