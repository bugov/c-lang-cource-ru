


## Упражнение: связный список

Что надо:

```
+-------------+    +-------------+
| DATA | NEXT |--->| DATA | NEXT |
+-------------+    +-------------+
```

```C
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  struct node * next;
  char val;
} node_t;

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

int main() {
  char c;
  node_t head = { .next = NULL, .val = ' ' };

  while ((c = getc(stdin)) != EOF) {
    add(&head, c);
  }

  print_list(&head);
}
```

## ДЗ

Добавить функции:
- index (-1, если не найден, сдвиг от head - иначе)
- remove_first / remove_last - параметр - значение, которое надо удалить из списка.

Альтернативное задание:
- Реализовать декартово дерево: добавление элемента, поиск элемента, удаление элемента
