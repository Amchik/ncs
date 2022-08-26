#include <stdio.h>
#include <pthread.h> /* -pthread */
#include <signal.h>
#include <string.h>
#include <unistd.h>

void handle(int code) {
  printf("Thread #%lu recieved signal %d (%s)\n", pthread_self(), code, strsignal(code));
  pthread_exit((void*)0x1488);
}

void *thread(void *_data) {
  int *p;

  signal(SIGSEGV, handle);

  p = 0;
  *p = 4;

  return (void*)p;
}

int main() {
  pthread_t t;
  void *ret;

  pthread_create(&t, 0, thread, 0);
  puts("Joining thread...");
  pthread_join(t, &ret);
  puts("Thread finished!");

  printf("Recieved pointer: %p\n", ret);
}
