# "Линковка" во время работы программы

Библиотеки Linux:
- Статические
- Общие (shared)
  + Динамическая линковка (компановка)
  + Динамическая загрузка

Для реализации динамической загрузки существует интерфейс динамической загрузки (Dynamic Loading API), дающий приложению пользователя возможность использовать совместно используемые библиотеки. Этот интерфейс невелик, однако он реализует все необходимое, беря всю "черную" работу на себя.

`libdl` – набор функций в системной динамической библиотеке
при помощи которых можно загрузить произвольную библиотеку в память,
просмотреть какие символы (в том числе функции) есть в этой библиотеке,
а затем вызвать их исполнение.

Нам понядобятся `dlopen()` и `dlsym()` из `<dlfcn.h>`:

```C
#include <dlfcn.h>
#include "linked_list.h"

void (* print_list)(node_t *head);
void (* add)(node_t * head, char val);

int load_library() {
  void * lib = dlopen("./liblinkedlist.so", RTLD_LAZY);
  if (lib == NULL) {
    return 1;
  }

  add = dlsym(lib, "add");
  print_list = dlsym(lib, "print_list");

  if (add == NULL) {
    return 2;
  }

  if (print_list == NULL) {
    return 3;
  }

  return 0;
}

int main() {
  int err = load_library();
  if (err) {
    printf("Library load error");
    return err;
  }

  char c;
  node_t head = { .next = NULL, .val = ' ' };

  while ((c = getc(stdin)) != EOF) {
    add(&head, c);
  }

  print_list(&head);
  return 0;
}
```

```
→ clang main.c
→ ./a.out
```

```
dlopen	Дает программе доступ к ELF-библиотеке
dlsym	Возвращает адрес функции из библиотеки, загруженной при помощи dlopen
dlerror	Возвращает текстовое описание последней возникшей ошибки
dlclose	Закрывает доступ к библиотеке
```
