#include <fcntl.h>
#include <linux/uinput.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main(void) {
  // Создаём дескриптор нашей реальной клавиатуры
  int hid_fd = open("/dev/input/event5", O_RDONLY);
  if (hid_fd < 0) {
    perror("open hid");
    exit(EXIT_FAILURE);
  }
  // Блокируем реальную клавиатуру для прочих потребителей
  // (теперь всё сюда)
  if (ioctl(hid_fd, EVIOCGRAB, 1) < 0) {
    perror("evioc grab");
    exit(EXIT_FAILURE);
  }

  // Создаём дескриптор нашего виртуального устройства
  int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

  // Настраиваем с помощью ioctl()
  // (эта функция отправляет в ядро команды, касающиеся устройства)

  // Разрешаем тип событий «клавиши вообще»
  ioctl(fd, UI_SET_EVBIT, EV_KEY);

  // Конкретно разрешаем клавишу A
  ioctl(fd, UI_SET_KEYBIT, KEY_A);

  // Определяем параметры устройства
  struct uinput_setup usetup;
  memset(&usetup, 0, sizeof(usetup));
  usetup.id.bustype = BUS_USB;
  usetup.id.vendor = 0x1234;  /* без смысла */
  usetup.id.product = 0x5678; /* без смысла */
  strcpy(usetup.name, "Example device");

  // Регистрируем устройство
  ioctl(fd, UI_DEV_SETUP, &usetup);
  ioctl(fd, UI_DEV_CREATE);

  // Даём ядру время увидеть и принять устройство
  // А нам в этом время нужно запустить sudo evtest
  sleep(10);

  // Прокидываем реальные события в виртуальную клавиатуру
  struct input_event ev;
  int counter = 0;
  while (read(hid_fd, &ev, sizeof(ev)) > 0) {
    fprintf(stderr, "%d • ", counter++);  // это же обычная программа
    write(fd, &ev, sizeof(ev));
  }

  // Даём ядру время переварить события
  sleep(1);

  // Удаляем устройство
  ioctl(fd, UI_DEV_DESTROY);

  // Закрываем дескриптор устройства
  close(fd);

  return 0;
}
