### Выравнивание структур

> – Выравнивание.
> – Что?
> – Выравнивание!
> - ?
> – Ну это там где потраченные на ничто байты, порядок, ебать его в сраку, структур... Выравнивание!

И всё же у нас "под капотом" всё те же байты. Поэтому посмотрим, как
устроены эти самые структуры.

```C
#include <stdio.h>
#include <math.h>
// Подключаем для memcpy (да, неочевидно, но именно в "строках")
// Как будто qsort должен был быть в stdlib.h
#include <string.h>

typedef struct Point2d Point2d;
struct Point2d {
  int x;
  int y;
};

typedef struct User {
  unsigned char age;
  unsigned int ip;
  char name[16];
} User;

typedef struct Example {
  char symbol;
  short num1;
  short num2;  // Смотрим содержимое с этим полем и без него
  int some_int;
} Example;

void print_hex(void *a, int size) {
  char *byte = (char *)a;

  for (int i = 0; i < size; ++i) {
    printf("%02hhx ", *(byte + i));
  }

  printf("\n");
}

int main() {
  char buf[100];
  Point2d a = { .y = -1, .x = 1};
  // Ожидаемо, 8 байт (по 4 за каждый int)
  printf("Point2d size: %lu\n", sizeof(a));
  memcpy(buf, &a, 8);
  print_hex(buf, 8);

  User u = { .name = "Ivanov Ivan", .age = 21, .ip = 0x7f000001 };
  // Упс
  printf("User size: %lu (21 expected)\n", sizeof(u));
  memcpy(buf, &u, sizeof(u));
  print_hex(buf, sizeof(u));

  Example e1 = { 'a', 1, 2, -1 };
  printf("Example size: %lu\n", sizeof(e1));
  memcpy(buf, &e1, sizeof(e1));
  print_hex(buf, sizeof(e1));
}
```

Получаем 24 вместо 21 байта (если sizeof(int) == 4). Если присмотреться
к байтам:

```
15 00 00 00  // age
01 00 00 7f  // ip
49 76 61 6e 6f 76 20 49 76 61 6e 00 00 00 00 00  // name
```

Результаты с 2 short:

```
61
00     // ?
01 00
02 00
00 00  // ?
ff ff ff ff
```

С 1 short:

```
61
00     // ?
01 00
ff ff ff ff
```

Можно заметить, что 1-байтовые поля не выравниваются, 2-байтовые — выравниваются
на чётные позиции, 4-байтовые — на позиции кратные четырём. Понятнее
"листинг" с примером 2-х short полей можно записать так:

```
00: 61
01: 00     // ?
02: 01 00
04: 02 00
06: 00 00  // ?
08: ff ff ff ff
```

– где числа до ":" – отсутп в байтах от начала структуры. Именно по этому
отсутпу и выравниваются поля структуры.

**Самообучение**: разобраться с выравниванием в структуре User.

TODO: Дописать объяснение с картинами причин выравнивания, какая оптимизация идёт. Да, я забил на это и в версии 0.02

