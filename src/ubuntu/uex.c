#include <fcntl.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

// Функция отправки события
void emit(int fd, int type, int code, int val) {
  struct input_event ie;

  ie.type = type;  // типа пакета
  ie.code = code;  // код (для нас — код клавиши)
  ie.value = val;  // 0, 1 или 2
  /* timestamp values below are ignored */
  ie.time.tv_sec = 0;
  ie.time.tv_usec = 0;

  write(fd, &ie, sizeof(ie));
}

int main(void) {
  // Создаём дескриптор нашего устройства
  int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

  // Настраиваем с помощью ioctl()
  // (эта функция отправляет в ядро команды, касающиеся устройства)

  // Разрешаем тип событий «клавиши вообще»
  ioctl(fd, UI_SET_EVBIT, EV_KEY);

  // Конкретно разрешаем клавишу B
  ioctl(fd, UI_SET_KEYBIT, KEY_B);

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

  emit(fd, EV_KEY, KEY_B, 1);       // нажимаем (начало события)
  emit(fd, EV_SYN, SYN_REPORT, 0);  // закончили нажимать (конец события)
  emit(fd, EV_KEY, KEY_B, 0);       // отпускаем (начало события)
  emit(fd, EV_SYN, SYN_REPORT, 0);  // закончили отпускать (конец события)


  // Даём ядру время переварить события
  sleep(1);

  // Удаляем устройство
  ioctl(fd, UI_DEV_DESTROY);

  // Закрываем дескриптор устройства
  close(fd);

  return 0;
}
