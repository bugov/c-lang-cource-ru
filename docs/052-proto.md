## Прототипы функций

Как можно увидеть, возникли проблемы: на момент использования функций в main
они ещё не определены. Всё же компилятор Си однопроходный...

Однако, мы можем описать их прототипы, чтобы
при компилятор понимал: какие типы значений ожидаются функциями
и какие типы значений вернут функции. Ещё немного изменим наш код:

```C
#include <stdio.h>
#include <stdlib.h>

/**
 * Прототипы
 */
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
