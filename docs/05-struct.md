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
  char *name;
} User;

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
  memcpy(buf, &a, sizeof(u));
  print_hex(buf, sizeof(u));
}
```

Получаем 24 вместо 21 байта (если sizeof(int) == 4). Если присмотреться
к байтам:

```
01 00 00 00  // ?
ff ff ff ff  // ?
15 00 00 00  // age
01 00 00 7f  // ip
4a 6f 65 00 00 00 00 00  // указатель на name
```

## Enum - перечисляемый тип данных

Enumerated тип данных мы могли уже видеть в других языках. Используются они
для

## Битовые поля
## Union
### Упражнение: объединяем структуры TCP и UDP заголовков.
