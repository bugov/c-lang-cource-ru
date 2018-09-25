## Сделаем свою небольшую библиотеку

Вспомним связный список, который мы реализовали в теме про динамическую память.

Для начала определим "правила игры" / "контракт" нашей библиотеки в
заголовочном файле `linked_list.h`

```C
#ifndef	_LINKED_LIST_H_
#define	_LINKED_LIST_H_

#include <stdio.h>

typedef struct node {
  struct node * next;
  char val;
} node_t;

void print_list(node_t *head);
void add(node_t * head, char val);

#endif /* _LINKED_LIST_H_ */
```

Ну и сам код, "реализация контракта" `linked_list.c`:

```C
#include "linked_list.h"
#include <stdlib.h>

void print_list(node_t *head) {
  node_t * current = head;

  while (current != NULL) {
    printf("%c->", current->val);
    current = current->next;
  }

  printf("\n");
}

void add(node_t * head, char val) {
  node_t * current = head;
  while (current->next != NULL) {
    current = current->next;
  }

  current->next = (node_t * )malloc(sizeof(node_t));
  current->next->val = val;
  current->next->next = NULL;
}
```

И `main.c`, который будет использовать нашу библиотеку:

```C
#include "linked_list.h"

int main() {
  char c;
  node_t head = { .next = NULL, .val = ' ' };

  while ((c = getc(stdin)) != EOF) {
    add(&head, c);
  }

  print_list(&head);
}
```

Начинаем собирать. Сначала объекты нашей библиотеки и основного файла:

```
→ clang -c linked_list.c -o linked_list.o
→ clang -c main.c -o main.o
```

В сущности, именно сейчас и происходит "компиляция". Компилятор берет файлы
с исходным кодом и получает из них объектные файлы.

Далее, собираем их в программу:

```
→ clang linked_list.o main.o -o linked_list_example
```

Линковщик берет объектные файлы и получает из них исполняемый файл.
Сборка = компиляция + линковка.

## Задание на подумать/проверить

Что будет, если мы в `linked_list.c`:
1. Не будем подключать `linked_list.h`, а определим `node_t` и подключим `stdio.h`.
2. Добавим в конец структуры Си-файла ещё одно поле, а в .h-файле не изменим
   структуру.
3. Добавим в начало структуры Си-файла ещё одно поле, а в .h-файле не изменим
   структуру.

## ДЗ

Написать библиотеку для реализации аналога утилиты xxd для структур Си.
