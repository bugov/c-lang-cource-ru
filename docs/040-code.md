# Примеры решений ДЗ

Примеры взяты из решений студентов. "Рабский труд" и всё такое.

## 1ая тема

Программа для проверки прямоугольности треугольника:
- На вход даются 3 числа - длины 3-х сторон треугольника.
- Вывести "True", если треугольник прямоугольный, иначе "False".

Тесты:
- 3, 4, 5
- 20, 4, 5
- 1.2345678, 2.3456789, 2.650729551406942

```C
#include <stdio.h>

int main(void){
    float a;
    float b;
    float c;
    float epsilon = 0.001;

    scanf("%f %f %f", &a, &b, &c);

    // Обработка ситуации с неправильным треугольником
    if (a + b - c < epsilon || a + c - b < epsilon || b + c - a < epsilon){
        puts("Uncorrect");
	// Это исключительная ситуация - указываем это кодом ошибки.
        return 1;
    }
    
    // Проверки по теореме Пифагора

    if (a * a + b * b - c * c < epsilon){
        puts("True");
        return 0;
    }

    if (a * a + c * c - b * b < epsilon){
        puts("True");
        return 0;
    }

    if (c * c + b * b - a * a < epsilon){
        puts("True");
        return 0;
    }

    // Если ничто не подошло, в рамках описанного контракта - вернём False
    puts("False");
    return 0;
}
```

## 2ая тема

Написать утилиту, позволяющую считать статистику по символам: цифры, латиница.

Пример работы:

```Bash
→ cat /etc/passwd | ./counter
Digits: 12
Latin symbols: 112
```

Подсказка. Читать символы можно функцией getc из stdio, читать нужно из stdin,
а символ конца ввода – EOF. Например:

```C
while ((c = getc(stdin)) != EOF)
```

```C
#include <stdio.h>

int main(void){
    int count_digital = 0;
    int count_character = 0;
    char c;

    while ((c = getc(stdin)) != EOF){
	// Используем тот факт, что символы от 'a' до 'z' идут порят.
	// Аналогично с от 'A' до 'Z'.
	
	// И помним, что двойные кавычки для строк, одинарные - для символов.
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
            count_character += 1;
        }

	// От '0' до '9' символы также идут подряд.
        if (c >= '0' && c <= '9'){
            count_digital += 1;
        }
    }

    printf("Count of digital: %d\n", count_digital);
    printf("Count of character: %d\n", count_character);
    return 0;
}
```

# 3ая тема

- Написать вывод таблицы квадратов в псевдографике.

  ![Таблица](http://matematikaiskusstvo.ru/images/tb2.jpg)
  + балл за реализацию на указателях

```C
#include <stdio.h>

int main(void){
    puts("Table of squares of natural numbers from 10 to 99");
    int size = 10;
    int nums[size][size];
    int k = 0;

    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size; ++j) {
            nums[i][j] = ((i + 1) * size + j) * ((i + 1) * size + j);
        }
    }

    for (int i = 0; i < size ; ++i){
        printf("%4d ", i);
    }
    printf("\n");

    for (int (*i)[size] = nums; i != nums + size - 1; ++i) {
        for (int *j = *i; j != *i + size; ++j) {
            if (j - (*i) == 0){
                printf("%d ", k + 1);
                k++;
            }
            printf("%4d ", *j);
        }
        printf("\n");
    }

    return 0;
}
```

- Написать функцию `strncat`.

```C
#include <stdio.h>
#include <string.h>

char* sstrncat(char *destptr, const char *srcptr, size_t num){
    int length = strlen(destptr);

    for (int i = 0; i < num; ++i){
        destptr[length + i] = srcptr[i];
    }

    // Не забываем про нуль-байт
    destptr[length + num] = 0;

    return destptr;
}

int main(void){
    char str1[40];
    char str2[40];
    int size;

    scanf("%s %s %d", &str1, &str2, &size);

    sstrncat(str1, str2, size);

    printf ("Result: %s\n", str1);
    return 0;
}
```

