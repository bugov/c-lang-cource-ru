## Enum - перечисляемый тип данных

Enumerated тип данных мы могли уже видеть в других языках. Используются они
для типов, где количество возможных значений ограничено. Например, мы можем
"изобрести" булевый тип данных:

```C
typedef enum Bool {
  FALSE,
  TRUE
} Bool;
```

Значения полей стартуют с 0 и увеличиваются на 1 ("сахар" над целочисленным типом).
Таким образом наш FALSE и TRUE будут работать адекватно.
За исключением случаев, когда мы другую "истину" попытаемся сравнить с нашей.

Рассмотрим ещё один пример – работаем с месяцами:

```C
#include <stdio.h>

const char *month_ru[] = {
  "",  // Делаем "отступ". Месяцы же с 1!
  "Январь", "Февраль",
  "Март", "Апрель", "Май",
  "Июнь", "Июль", "Август",
  "Сентябрь", "Октябрь", "Ноябрь",
  "Декабрь",
};

enum Month {
  // Указываем, что поле 1, а не 0 как по умолчанию.
  // Следующие значение продолжатся с 2-х.
  JANUARY = 1,
  FEBRUARY,
  MARCH, APRIL, MAY,
  JUNE, JULY, AUGUST,
  SEPTEMBER, OCTOBER, NOVEMBER,
  DECEMBER,
};

int main() {
  enum Month mon;

  // Воспользуемся тем,
  // что мы знаем внутреннее представление enum
  scanf("%d", &mon);
  puts(month_ru[mon]);

  if (mon >= JUNE && mon <= AUGUST) {
    puts("Лето!");
  }

  return 0;
}
```

Попробуйте сломать данную программу. Благо – мы для краткости и понятности
не добавили никакой обработки исключительных ситуаций.

Также стоит знать о том, что мы можем для разных Enum значений задачть одно и
то же число. Например:

```C
typedef enum Bool {
  FALSE = 0,
  TRUE = 1,
  false = 0,
  true = 1,
} Bool;
```