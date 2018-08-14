# Структуры и объединения

Как мы уже ни раз были свидетелями: всё, что у нас есть в Си – данные.
Однако, встроенных типов данных не так уж и много, а хочется удобств...
И в каком-нибудь Java, Python, C# мы бы написали класс. В Си же у нас будут
структуры!

## Структуры

### struct - структура двумерной точки

Структуры Си – композитный тип данных – возможность составлять из уже
имеющихся типов более сложные. Никакой инкапсуляции и сокрытия деталей реализации
– не забываем – просто представление байтов.

В распоряжении также указатели на структуры – можно делать рекурсивные типы данных,
например, структуру типа дерева.

Для примера рассмотрим двумерную точку:

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

То есть в общем случае мы можем дать другое название какому-либо типу:

```C
typedef <какой-то тип> <его ещё одно название>;
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
