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

```Bash
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






## Void
