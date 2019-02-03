# Многопоточность в Linux и Си

> У программиста была проблема, которую он решил с помощью тредов.
> Теперь е проблемы. у него дв

Одним из решений, позволяющих в рамках одной программы / сервиса реализовать
конкурентность в современных операционных системах, является многопоточность.

Так, например, нам нужно параллельно обрабатывать запросы от нескольких
пользователей, имея один процессор. вы можете на каждого пользователя в программе выделить по потоку.
Код обработчика будет запущен для каждого пользователя, а распределением процессорного
времени будет заниматься операционная система.

С другой стороны, если у вас несколько процессоров (или ядер), вы можете захотеть
в рамках одной программы использовать их все. Например, это могут быть довольно
тяжёлые вычисления. Тогда вы можете под каждое ядро выделить по потоку и
использовать память процесса для синхронизации потоков.

Если рассматривать в целом конкуретность в программировании, можно выделить
основные подходы:

- Процессы – запустить несколько процессов (например, через системный вызов `fork`),
  общаться между ними средствами межпроцессного взаимодействия (IPC, например, shared memory,
  pipe, socket).
- Треды, они же потоки, они же "нити" – о них поговорили выше.
- Асинхронная работа – мультиплексирование потоков ввода/вывода – используем
  системные вызовы epoll (Linux), kqueue (BSD), WSAPoll (Windows). Об этом –
  в будущей теме.

Рассмотрим на примере функции для работы с POSIX-тредами (`man pthread`).
Для примера напишем простой TCP чат.

```C
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define PORT 9009

#define error(...) { \
   fprintf(stderr, __VA_ARGS__); \
   fprintf(stderr, "\n"); \
   fflush(stderr); \
   exit(1); \
}

void* client_handler(void* arg) {
  char buf[BUF_SIZE];
  int client_fd = *(int *)arg;
  int bytes_received;

  while (1) {
    bytes_received = recv(client_fd, buf, sizeof(buf), 0);
    if (bytes_received <= 0) {
      break;
    }

    send(client_fd, buf, bytes_received, 0);
  }
  close(client_fd);
  return arg;
}

int main() {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    error("Не смог создать сокет сервера");
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  int opt_val = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    error("Не удалось забиндить сокет");
  }
  if (listen(server_fd, 10) < 0) {
    error("Не удалось прослушивать сокет");
  }

  printf("Сервер запущен на * : %d", PORT);

  int client_fd;
  struct sockaddr_in client_addr;
  socklen_t addr_size = sizeof(client_addr);
  while (1) {
    pthread_t thread;
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);
    if (client_fd < 0) {
      error("Не могу принять соединение от клиента");
    }

    if (pthread_create(&thread, NULL, client_handler, &client_fd) != 0) {
      error("Ошибка создния треда");
    } else {
      pthread_detach(thread);
    }
  }

  return 0;
}
```
