# Структуры и объединения

Как мы уже не раз были свидетелями: всё, что у нас есть в Си – данные.
Однако, встроенных типов данных не так уж и много, а хочется удобств...
И в каком-нибудь Java, Python, C# мы бы написали класс.

## Структуры

### struct - структура двумерной точки

Структуры Си – композитный тип данных – возможность составлять из уже
имеющихся типов более сложные. Никакой инкапсуляции и сокрытия деталей реализации
– не забываем – просто представление байтов.

В распоряжении также указатели на структуры – можно делать рекурсию, например,
структуру типа дерева.

```C
#include <stdio.h>

struct Point2d {
  int x;
  int y;
};

void print_point(struct Point2d point) {
  printf("<Point %d, %d>\n", point.x, point.y);
}

int main() {
  struct Point2d point = { -1, 1 };
  print_point(point);
}
```

### typedef

Аналогично, но используем typedef для удобства – создадим алиас для типа, чтобы
не писать постоянно `struct`.

```C
#include <stdio.h>

typedef struct Point2d {
  int x;
  int y;
} Point2d;

void print_point(Point2d point) {
  printf("<Point %d, %d>\n", point.x, point.y);
}

int main() {
  Point2d point = { .y = -1, .x = 1};  // Ещё один способ инициализации
  print_point(point);

  Point2d point2 = point;  // Копирование
  point2.y = 255;
  print_point(point);

  Point2d *point3 = &point;  //*
  point3->x = 343;  // Разыменование - очередной синтаксический сахар
  print_point(point);
}
```

Также можно делать алиасы для более простых типов данных, например, использованный
ранее тип size_t:

```C
typedef unsigned long size_t;
```

### Упражнение: считаем расстояние между точками

```C
#include <stdio.h>
#include <math.h>

typedef struct Point2d Point2d;
struct Point2d {
  int x;
  int y;
};

int distance(Point2d *point_1, Point2d *point_2) {
  double x = (double)point_1->x - (double)point_2->x;
  double y = (double)point_1->y - (double)point_2->y;
  return sqrt(x * x + y * y);
}

int main() {
  Point2d a = { .y = -1, .x = 1};
  Point2d b = { .y = 1, .x = 1};
  printf("%d\n", distance(&a, &b));
}
```

### Выравнивание

И всё же у нас "под капотом" всё те же байты. Поэтому посмотрим, как
устроены эти самые структуры.

```C
#include <stdio.h>
#include <math.h>
// Подключаем для memcpy (да, неочевидно, но именно в "строках")
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

TODO: Дописать объяснение с картинами причин выравнивания, какая оптимизация идёт.

## Enum - перечисляемый тип данных

Enumerated тип данных мы могли уже видеть в других языках. Используются они
для типов, где количество возможных значений ограничено. Например:

```C
typedef enum Bool {
  FALSE,
  TRUE
} Bool;
```

```C
#include <stdio.h>

char *ErrorNames[] = {
  "",
  "Some error",
  "Another error"
};

enum Errors {
  SOME_ERROR = 1,
  ANOTHER_ERROR
};

int main() {
  enum Errors err = ANOTHER_ERROR;
  printf("Size: %lu\nValue: %lu\n", sizeof(err), err);
  printf(ErrorNames[err]);
  return err;
}
```

## Битовые поля

Как понятно из названия, нужны они для доступа для отдельных битов (или набора битов).
Реализованы они в Си на базе структур:

```
struct <имя> {
    <тип> <имя>: <размер>;
    ...
}
```

```C
#include <stdio.h>

struct SomeData {
    unsigned a: 1;
    unsigned b: 2;
    unsigned c: 4;
    unsigned d: 1;
    unsigned e: 32;  // Добавить/убрать для просмотра разницы
};

int main() {
  struct SomeData data = { 0, 3, 12, 1, 1234 };
  printf("%lu\n", sizeof(data));
  printf("%d\n", data.a);
  printf("%d\n", data.b);
  printf("%d\n", data.c);
  printf("%d\n", data.d);
  printf("%d\n", data.e);
  return 0;
}
```

## Union



### Упражнение: объединяем структуры TCP и UDP заголовков.
