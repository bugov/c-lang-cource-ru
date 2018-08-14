

## Строки

На самом деле, строки в Си – это массив символов.

```C
    char text[8] = "Hello C";
```

Зачем 8, а не 7?

Всё потому что требуется ещё и символ \0 в конце строки – символ конца строки в
Си-строках.

strlen:

```C
#include <stdio.h>

int main(void) {
    char *text = "Hello world";

    int i = 0;
    while (text[i++] != 0);

    printf("%d", i - 1);
    return 0;
}
```

```C
    printf("%s\n", text);
    printf("%d\n", strlen(text));
```

```C
    if (strncmp(name, "Hello", 5) == 0) {
        printf("Hello!\n");
    } else {
        printf("Not equal.\n");
    }

    char dest[10]="Hello";
    char src[]="World";
    strncat(dest, src, 3);
    printf("%s\n", dest);
```

# ДЗ

- Написать вывод таблицы квадратов в псевдографике.

  ![Таблица](http://matematikaiskusstvo.ru/images/tb2.jpg)
  + балл за реализацию на указателях
- Написать функцию `strncat`.
