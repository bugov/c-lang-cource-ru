# Функции и структура программы

Как и во многих других языках программирования, в Си имеются средства
создания функций. Используются они как и в других ЯП для организации
кода и повторного использования блоков операторов.

```
<возвращаемый тип> <имя функции>(<тип1> <арг1>, <тип1> <арг2>, ...) {
    <тело функции>
}
```

Напишем пару функций:

```C
#include <stdio.h>
#include <stdlib.h>

int get_input() {
    puts("Please enter number less than 100 and more -100:");

    int input;
    scanf("%d", &input);

    if (input >= 100) {
        printf("Expected number less than 100, but got: %d\n", input);
        exit(1);
    }

    if (input <= -100) {
        printf("Expected number greater than -100, but got: %d\n", input);
        exit(1);
    }

    return input;
}

void print_output(int value) {
    puts("=== output ===");
    printf("%d\n", value);
    puts("==============");
}

int calc_value(value) {
    return value * value;
}

int main(void) {
    int arg = get_input();
    int value = calc_value(arg);
    print_output(value);
    return 0;
}
```

Воспользуемся кодом:

```Bash
→ clang 03-simple-funcs.c
→ ./a.out
Please enter number less than 100 and more -100:
123
Expected number less than 100, but got: 123
→ ./a.out
Please enter number less than 100 and more -100:
-123
Expected number greater than -100, but got: -123
→ ./a.out
Please enter number less than 100 and more -100:
12
=== output ===
144
==============
```

Однако, если переместить main выше определения функций, получим:

```
→ clang 03-simple-funcs.c
03-simple-funcs.c:6:15: warning: implicit declaration of function 'get_input' is invalid in C99 [-Wimplicit-function-declaration]
    int arg = get_input();
              ^
03-simple-funcs.c:7:17: warning: implicit declaration of function 'calc_value' is invalid in C99 [-Wimplicit-function-declaration]
    int value = calc_value(arg);
                ^
03-simple-funcs.c:8:5: warning: implicit declaration of function 'print_output' is invalid in C99 [-Wimplicit-function-declaration]
    print_output(value);
    ^
03-simple-funcs.c:33:6: error: conflicting types for 'print_output'
void print_output(int value) {
     ^
03-simple-funcs.c:8:5: note: previous implicit declaration is here
    print_output(value);
    ^
3 warnings and 1 error generated.
```

## Прототипы функций

Как можно увидеть, возникли проблемы: на момент использования функций в main
они ещё не определены. Однако, мы можем описать их прототипы, чтобы
при компилятор понимал: какие типы значений ожидаются функциями
и какие типы значений вернут функции. Ещё немного изменим наш код:

```C
#include <stdio.h>
#include <stdlib.h>

int get_input();
void print_output(int);
int calc_value(int);

int main(void) {
    int arg = get_input();
    int value = calc_value(arg);
    print_output(value);
    return 0;
}

int get_input() {
    puts("Please enter number less than 100 and more -100:");

    int input;
    scanf("%d", &input);

    if (input >= 100) {
        printf("Expected number less than 100, but got: %d\n", input);
        exit(1);
    }

    if (input <= -100) {
        printf("Expected number greater than -100, but got: %d\n", input);
        exit(1);
    }

    return input;
}

void print_output(int value) {
    puts("=== output ===");
    printf("%d\n", value);
    puts("==============");
}

int calc_value(int value) {
    return value * value;
}
```

## Передача параметров

Уже было видно в предыдущем примере, как можно передавать данные в функции

```C
// Пототипы
void print_output(int);
int calc_value(int);

...

// main
int value = calc_value(arg);
print_output(value);
```

Сами значения копируются, а после выхода из функций удаляются (помимо return
значения, которое также копируется).

Однако, мы помним, что в Си у нас всё данные, даже адреса. Поэтому мы можем
передать адрес на данные и модифицировать данные вне функции.

Вспомним уже использованную функцию scanf - форматированный ввод со
стандартного ввода:

```C
#include <stdio.h>

int main(void) {
    int num;
    scanf("%d", &d);
    printf("%d", d);
    return 0;
}
```

Нам **нужен** указатель на переменную в scanf, чтобы записать по адресу
значение во вне. Нам **не нужен** указатель для printf – нам нужно
получить значение, вывести его – изменять его значение вне функции не нужно.

## Void

void - необычный специфиактор типа. Иногда он имеет смысл "что-то", иногда -
"ничто".

1. Тип возвращаемого функцией значения:
   ```C
    #include <stdio.h>
    #include <string.h>

    void beautiful_print(char *text) {
      for (int i = 0; i < strlen(text) + 4; ++i) {
        printf("*");
      }

      printf("\n* %s *\n", text);

      for (int i = 0; i < strlen(text) + 4; ++i) {
        printf("*");
      }

      printf("\n");
    }

    int main(void) {
      beautiful_print("Hello world");
      return 0;
    }
   ```
2. Декларация функции – указание на то, что функция не имеет параметров:
    ```C
     #include <stdio.h>

     int beautiful_read(void) {
       int digit;
       printf("Enter a number: ");
       scanf("%d\n", &digit);
       return digit;
     }

     int main(void) {
        int num = beautiful_read("some param");
       return 0;
     }
    ```
   Получаем:
   ```
   → clang 04-void-def.c
   04-void-def.c:11:34: error: too many arguments to function call, expected 0, have 1
          int num = beautiful_read("some param");
                    ~~~~~~~~~~~~~~ ^~~~~~~~~~~~
   04-void-def.c:3:6: note: 'beautiful_read' declared here
       int beautiful_read(void) {
       ^
   1 error generated.
   ```
   В случае, если не указываем void в декларации функции - только warning:
   ```
   → clang 04-void-def.c
    04-void-def.c:11:46: warning: too many arguments in call to 'beautiful_read'
            int num = beautiful_read("some param");
                      ~~~~~~~~~~~~~~             ^
    1 warning generated.
   ```
3. Тип указателя "нечто". Полезно, когда могут быть использованы параметры
   разных типов:
   ```C
   #include <stdio.h>
   #include <string.h>

   void print_hex (void *a, int size) {
     char *byte = (char *)a;

     for (int i = 0; i < size; ++i) {
       printf("%02x ", *(byte + i));
     }

     printf("\n");
   }

   int main(void) {
     int a = 1025;
     print_hex(&a, sizeof(a));
     char *b = "abcd";
     print_hex(b, strlen(b));
     return 0;
   } //*
   ```

## Колбеки и функции высших порядков

Опять же, в Си нет разграничения данных и кода – код также данные,
на которые мы также можем получить указатель и передать переменной.

### Функции как аргументы функций

Пожалуй, самый простой пример, когда полезно использовать функцию
как агрумент другой функции – сортировка. Например, когда мы в Python
сортируем итерабельные объекты:

```Python
>>> lst = [122, 43, 1, 452, 87, 5]
>>> sorted(lst, key=lambda x: len(str(x)))
[1, 5, 43, 87, 122, 452]
```

– мы передаём параметром функцию, которая "оценивает" каждый элемент на
"больше-меньше". Другой подход (более каноничный) мы можем увидеть
в языке Perl:

```Perl
my @lst = (122, 43, 1, 452, 87, 5);
@result = sort { length($a) <=> length($b) } @lst;
print join(" ", @result);
^D
1 5 43 87 122 452
```

– здесь мы передаём функции `sort` функцию сравнения от двух сравниваемых
значений `$a` и `$b`, которая вернёт `-1` - если `$b > $a`, `0` - если `$b == $a`,
`1` - если `$a > $b`.

На Си это будет выглядеть так:

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmpfunc (const void *a, const void *b) {
    char a_str[10];
    char b_str[10];

    sprintf(a_str, "%d", *(int *)a);
    sprintf(b_str, "%d", *(int *)b);

    return strlen(a_str) - strlen(b_str);   //*
}

int main () {
    int values[] = { 122, 43, 1, 452, 87, 5 };

    qsort(values, 6, sizeof(int), cmpfunc);

    for(int n = 0; n < 6; n++) {   
        printf("%d ", values[n]);
    }

    return(0);
}
```

Прототип функции `qsort` (`man qsort`):

```C
void qsort(         // Функция ничего не вернёт
    void *base,     // Указатель на что-либо
    size_t nitems,  // Беззнаковое целое для "размеров"
    size_t size,
    int (*compar)(const void *, const void *)
);
```

Параметры:
* base − указатель на первый элемент массива, который мы сортируем.
* nitems − количество элементов массива.
* size − размер в байтах каждого элемента массива.
* compar − функция сравнения 2-х элементов.

### Напишем свою функцию высшего порядка

```C
#include <stdio.h>
#include <stdlib.h>

void map(
  void *from,
  void *to,
  size_t count,
  size_t size,
  void (*modify)(void *, void *)
) {
  /**
   * Адресной арифметики на void-указателях нет.
   * Поэтому преобразуем в байт-указатели.
   */
  char *from_bytes = (char *)from;
  char *to_bytes = (char *)to;

  for (int i = 0; i < count; ++i) {
    modify(from_bytes + i * size, to_bytes + i * size);
  }
}

void modify_int(void *from, void *to) {
  int *a = (int *)from;
  int *b = (int *)to;
  *b = (*a) * (*a);
}

int main() {
  int a[] = {1, 2, 3, 4, 5};
  int b[5];

  map(a, b, 5, sizeof(int), modify_int);

  for (int i = 0; i < 5; ++i) {
    printf("%d\n", b[i]);
  }

  return 0;
}
```

### Callback

Часто бывает нужно выполнить какое-то действие после другого (что очевидно :)).
Что не очевидно – не всегда это можно сделать последовательными вызовами 2-х
функций. Подробнее это мы рассмотрим на практике в следующих темах –
"обратные вызовы" – частый инструмент API в Си библиотеках.

Чисто технически это не сильно отличается от функций высшего порядка.

## ДЗ

Написать функции filter, reduce - аналоги [функций
python](https://ru.wikipedia.org/wiki/Функциональное_программирование_на_Python).
