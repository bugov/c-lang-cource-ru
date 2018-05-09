# Многопоточность в Linux и Си

```C
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

/**
 * Небольшая функция для "обработки" ошибок
 * для работы с файловыми дескрипторами.
 * Если recv/send вернёт -1 (количество принятых/отправленных байт)
 * - очевидно, что-то пошло не так.
 */
int guard(int r, char * err) {
  if (r == -1) {
    /**
     * Функция perror интерпретирует значение глобальной переменной ERRNO
     * в строку и выводит эту строку на стандартный
     */
    perror(err);
    exit(1);
  }
  return r;
}

/**
 * Сама функция - обработчик каждого соединения.
 * в качестве аргумента мы передадим файловый дескриптор соединения.
 */
void * thread_func(void * arg) {
  intptr_t conn_fd = (int) arg;
  printf("Thread: serving fd %ld\n", conn_fd);
  char buf[1024];

  while (1) {
    /**
     * Напоминаю, что recv читает не то количество байт,
     * которое мы попросили 3им параметром, а не больше его.
     * Однако, для наших задач (echo-сервер), это не важно.
     */
    int bytes_received = guard(
      recv(conn_fd, buf, sizeof(buf), 0),
      "Could not recv"
    );

    /**
     * Если мы прочитали 0 байт - это знак того,
     * что соединение закрыто.
     */
    if (bytes_received == 0) {
      goto stop_serving;
    }

    /**
     * Высылаем назад то, что нам прислали.
     * На этот раз нам важно отправить всё,
     * ибо этот буфер будет перетёрт при следующем чтении.
     */
    int bytes_sent = 0;
    while (bytes_sent < bytes_received) {
      ssize_t bytes_sent_this_call = send(
        conn_fd,
        buf + bytes_sent,
        bytes_received - bytes_sent,
        0
      );

      if (bytes_sent_this_call == -1) {
        goto stop_serving;
      }

      bytes_sent += bytes_sent_this_call;
    }
  }

/**
 * Прибираемся за собой, чтобы не копить мусор в памяти.
 */
stop_serving:
  guard(close(conn_fd), "Could not close socket");
  printf("Thread: finished serving %ld\n", conn_fd);

  /**
   * Тред ничего не считал, вернуть ничего не хочет.
   */
  return NULL;
}

int main(void) {
  int listen_fd = guard(
    socket(AF_INET, SOCK_STREAM, 0),
    "Could not create TCP listening socket"
  );

  guard(listen(listen_fd, 100), "Could not listen");

  struct sockaddr_in addr;
  socklen_t addr_bytes = sizeof(addr);

  guard(
    getsockname(
      listen_fd,
      (struct sockaddr *) &addr,
      &addr_bytes
    ),
    "Could not get sock name"
  );

  printf("Listening on port %d\n", ntohs(addr.sin_port));

  while (1) {
    intptr_t conn_fd = guard(
      accept(listen_fd, NULL, NULL),
      "Could not accept"
    );

    pthread_t thread_id;
    int ret = pthread_create(
      &thread_id,
      NULL,
      thread_func,
      (void*) conn_fd
    );

    if (ret != 0) {
      printf("Error from pthread: %d\n", ret);
      exit(1);
    }
    printf("main: created thread to handle connection %ld\n", conn_fd);
  }
  return 0;
}
```
