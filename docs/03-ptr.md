# Массивы, строки, указатели

## Указатели

Указатель – это переменная, которая хранит адрес области памяти.
Указатель, как и переменная, имеет тип.

Синтаксис объявления указателей

```
<тип> *<имя>;
```

```C
float *a;
long long *b;
```

Два основных оператора для работы с указателями – это оператор & взятия адреса,
и оператор * разыменования.

```C
#include <stdio.h>

int main(void) {
    int A = 100;  // Значения
    int *p;       // Неинициализированный указатель
    printf("%p\n", p);

    p = &A;       // Инизиализаци

    printf("%p\n", p);
    printf("%d\n", sizeof(p));
    printf("%d\n", *p);

    // p = 200;   // Кладём адрес в указатель.
                  // Проблем не будет, пока не обратимся по этому адресу.
    *p = 200;     // По указателю кладём по указателю 200

    printf("%d\n", A);
    printf("%d", *p);

    int **double_ptr = &p;
    **double_ptr = 300;
    printf("%d", **double_ptr);

    return 0;
}
```

### Арифметика указателей

```C
#include <stdio.h>

int main(void) {
    int A = 2;
    int B = 3;
    int *p;

    p = &A;

    printf("%d\n", *p);   // Всё нормально
    p++;
    printf("%d\n", *p);   // Какая-то фигня

    puts("================");

    int C[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    p = &C[0];            // Эквивалентно p = C
    printf("%d\n", *p);
    p++;
    printf("%d\n", *p);   // C[1]
    p = p + 3;
    printf("%d\n", *p);   // C[4]

    p = p + 333333;
    printf("%d\n", *p);   // ?

    return 0;
}
```

`int C[8]`` – объявление массива. Элементы массива хранятся одним "куском"
– один за другим. Благодаря этому адресация в массиве быстрая (O(1)).
И благодаря этому можно провернуть этот "трюк" (на самом деле, это
как раз правда, а A[i] – "синтаксический сахар").

А сколько у нас размер `int`? Арифметика указателей зависит от типа указателя,
*не байтовая*.

### Преобразование указателей

Равно как мы можем преобразовывать одни типы к другим

```C
float b = 2.12;
int a = (int)b;
```

Также мы можем преобразовывать один тип указателей в другой:

```C
#include <stdio.h>

int main(void) {
    int a = 0x45464748;
    int *p = &a;
    char *q = (char *)p;

    printf(
        "%c %c %c %c\n",
        *q,
        *(q + 1),
        *(q + 2),
        *(q + 3)
    );
    return 0;
}
```

И получим вывод в представлении символов:

```
→ clang int-to-chars.c
→ ./a.out
H G F E
```

"H" = 0x48 и т.д.

## Массивы

```C
    int nums[10];
    nums[0] = 0;
    nums[1] = 1;
    nums[2] = 4;
    nums[3] = 9;
    nums[4] = 16;
```

```C
    int nums[10] = {
        0, 1, 4, 9, 16, 25, 36, 49, 64, 81
    };
```

Часто приятнее использовать циклы для работы с массивом:

```C
#include <stdio.h>

int main(void) {
    int nums[10];

    for (int i = 0; i < 10; ++i) {
        nums[i] = i * i;
    }

    for (int i = 0; i < 10; ++i) {
        printf("%d\n", nums[i]);
    }

    return 0;
}
```

Чему равен `nums[10]`?

Для разминки, то же самое на указателях:

```C
#include <stdio.h>

int main(void) {
    int nums[10];

    for (int *ptr = nums, i = 0; ptr < nums + 10; ++ptr, ++i) {
        *ptr = i * i;
    }

    for (int *ptr = nums; ptr < nums + 10; ++ptr) {
        printf("%d\n", *ptr);
    }

    return 0;
}
```

### Многоуровневые массивы

```C
#include <stdio.h>

int main(void) {
    int nums[4][10];

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 10; ++j) {
            nums[i][j] = 1;

            if (i > 0) {
                for (int k = 0; k < i; ++k) {
                    nums[i][j] *= j;
                }
            }
        }
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 10; ++j) {
            printf("%d ", nums[i][j]);
        }
        printf("\n");
    }

    return 0;
}
```

Или на указателях:

```C
#include <stdio.h>

int main(void) {
    int nums[4][10];

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 10; ++j) {
            nums[i][j] = 1;

            if (i > 0) {
                for (int k = 0; k < i; ++k) {
                    nums[i][j] *= j;
                }
            }
        }
    }

    for (int (*i)[10] = nums; i != nums + 4; ++i) {
        for (int *j = *i; j != *i + 10; ++j) {
            printf("%d ", *j);
        }
        printf("\n");
    }

    for (int *ptr = &nums[0][0], i = 0; i < 4 * 10; ++i) {
        printf("%d ", *(ptr + i));

        if (i % 10 == 9) {
            printf("\n");
        }
    }
}
```

Небольшое объяснение (запутывание):

```C
*(*ptr+1) = *(*(ptr + 0) + 1 ) =  *(ptr[0] + 1) = ptr[0][1]

nums[0][0] = *(*(nums))
nums[i][j] = *((*(nums)) + (i * COLS + j))
nums[i][j] = *(*(nums + i) + j)
nums[i][j] = *(nums[i] + j)
nums[i][j] = (*(nums + i))[j]
&nums[i][j] = ((*(nums)) + (i * COLS + j))
```

3d:

```C
#include<stdio.h>

int main() {
  int arr[2][3][2] = {
     {
       {5, 10},
       {6, 11},
       {7, 12},
     },
     {
       {20, 30},
       {21, 31},
       {22, 32},
     }
  };

  int i, j, k;
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
       for (k = 0; k < 2; k++) {
         printf("%d\t", *(*(*(arr + i) + j) +k));
       }

       printf("\n");
    }
  }

  for (i = 0; i < 2 * 3 * 2; i++) {
    printf("%d ", *(**arr + i));
  }
  printf("\n");
  return 0;
}
```

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
