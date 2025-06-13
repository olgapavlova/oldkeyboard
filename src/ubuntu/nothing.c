#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
  int fd = open("/dev/input/event5", O_RDONLY);
  if (fd < 0) { perror("open hid"); exit(1); }

  printf("?\n");
  struct input_event ev;
  int counter = 0;
  while (read(fd, &ev, sizeof(ev)) > 0) {
      printf("%d\n", counter);  // тут что угодно
      counter++;
  }
  printf("*\n");

  close(fd);
}
