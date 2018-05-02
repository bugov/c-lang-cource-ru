# Сделаем свою небольшую **общую** библиотеку

В предыдущей теме мы написали свою небольшую библиотеку для работы со
связным списком. Также рассмотрели вариант линковки прямо в проект объектныым
файлом – так называемая "статическая линковка". Однако, многие библиотеки
поставляются не в виде `.c` и `.h` файлов, а в виде `.so` файлов, а также
`.h` файлов, чтобы их использовать. Мы видели `.o` – object файлы, теперь –
`.so` – shared object.

Всё тот же `linked_list.h`

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

Ну и сам `linked_list.c`:

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

`main.c` нам пока не нужен.

Собираем:

```
→ clang --shared -fPIC linked_list.c -o liblinkedlist.so
```

```
→ clang main.c -fPIC -L. -llinkedlist -o linked_list
```

```
→ mv liblinkedlist.so liblinkedlist.so.1
```

```
→ ./linked_list
dyld: Library not loaded: liblinkedlist.so
 Referenced from: /$PWD/./linked_list
 Reason: image not found
Abort trap: 6
```

В Linux утилита `ldd`, в MacOS - `otool -L`

```
→ otool -L linked_list
linked_list:
	liblinkedlist.so (compatibility version 0.0.0, current version 0.0.0)
	/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1252.0.0)
```

```
→ nm liblinkedlist.so
0000000000000f10 T _add
                 U _malloc
0000000000000eb0 T _print_list
                 U _printf
                 U dyld_stub_binder
```

В Linux за динамическую линковку отвечает ld. Используются
- дефолтные пути (/usr/lib, /usr/local/lib)
- $LD_LIBRARY_PATH, ключ -L для указания, где искать
- $LD_PRELOAD
