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

Position Independent Code

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

В случае использования динамической компоновки ядро передает управление на динамический компоновщик (другое название – ELF-интерпретатор), который после собственной инициализации загружает указанные совместно используемые библиотеки (если они уже не в памяти). Далее динамический компоновщик производит необходимые перемещения (relocations), включая совместно используемые объекты, на которые ссылаются требуемые совместно используемые библиотеки. Путь, по которому система будет искать совместно используемые объекты, задается переменной среды LD_LIBRARY_PATH. Закончив с библиотеками, компоновщик отдает управление исходной программе, которая начинает выполнение.

В основе процесса перемещения (relocation) лежит косвенная адресация, которую обеспечивают две таблицы – глобальная таблица смещений (Global Offset Table, GOT) и таблица связывания процедур (Procedure Linkage Table, PLT). В этих таблицах содержатся адреса внешних функций и данных, которые ld-linux.so должен загрузить в процессе перемещения. Получается, что код, содержащий обращение к внешним функциям и, таким образом, ссылающийся на данные этих таблиц, остается неизменным – модифицировать требуется только таблицы. Перемещение может проходить либо сразу во время загрузки программы, либо когда понадобится нужная функция.

По завершении перемещения динамический компоновщик исполняет стартовый код каждой совместно используемой библиотеки (если этот код имеется), содержащий инициализацию и подготовку внутренних данных. Стартовый код определяется в секции .init ELF-файла. Во время выгрузки библиотеки может выполняться также и завершающий код, определяемый в секции .fini. Вызвав функции инициализации, динамический компоновщик отдает управление исходному исполняемому образу.
