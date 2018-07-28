## Void

> – А есть ли в Си generic-и?
> - Конечно! void pointer!
>
> – диалог с одной из пар.

void - необычный специфиактор типа. Иногда он имеет смысл "что-то", иногда -
"ничто".

Звучит немного запутанно, но что взять с бек-порта из C++...

Давайте разберёмся, когда `void` используется и чем он является!

### Тип возвращаемого функцией значения

Мы помним, что при описании функции, нам нужно указать тип возвращаемого значения.

Но если мы не хотим ничего возвращать? Тогда нам нужен `void`!

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

### Декларация функции – указание на то, что функция не имеет параметров

Нет, мы, конечно, можем просто указать пустые скобочки... Но это лишь
означает для Си, что мы не даём названия параметра передаваемым данным.

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

Уоу! Я не буду это собирать – ты допустил ошибку!

В случае, если не указываем void в декларации функции - только warning:

```
→ clang 04-void-def.c
04-void-def.c:11:46: warning: too many arguments in call to 'beautiful_read'
    int num = beautiful_read("some param");
	      ~~~~~~~~~~~~~~             ^
1 warning generated.
```

– тут что-то не особо понятное... Но технически всё нормально – собираю.

### Тип указателя "нечто".

А вот это просто нечто! Ой, то есть "замечательно"!

Именно они позволят нам писать более "гибкий" код, именно они позволят творить
полную дичь, также напоминающую generic типы из более высокоуровневых языков.

Использовать `void` полезно, когда могут быть использованы параметры разных типов:

```C
#include <stdio.h>
#include <string.h>

/**
 * Функция выводит hex представление данных.
 * Согласитесь, такая функция не нуждается в типе данных.
 * Опять же, она полезна нам для просмотра того треша, который будет в переменных.
 */
void print_hex(void *a, int size) {
  // Преобразуем void указатель в указатель на байт (он же – указатель на символ).
  // Всё потому что арифметика указателей для void не работает – это же "нечто".
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
}
```

