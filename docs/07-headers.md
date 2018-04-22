На данный момент мы изучили Си – в том плане, что можем писать программы
на Си практически любой сложности. А действительно: основные операции есть;
так как всё есть байт, можем хоть какие структуры создавать; да ещё
и динамически.

Однако, не это самое сложное и интересное в программировании. Сам по себе
язык мало что стоит. А вот "технлогический стек", если хотите, или же
умение обращаться с "батарейками" – вот где раскрывается любой язык!

Наше фееричное введение в наСИлие на этом этапе принимает ещё более ужасающие
обороты! Переходим к "мясу" – к практике, где эклектика тулзовин и археология
IT в своих соперниках имеет разве что аппаратные средства и саму физику!

*Дальше идут скорее "пометки на полях" – набор пионера, чтобы выжить в джунглях
программ на Си.*

# Заголовочные файлы

Уже ни раз мы подключали функции `stdio` с помощью команды препроцессора
`#include`. Сам заголовочный файл можно посмотреть:

```
→ cat /usr/include/stdio.h
```

И увидеть примерно следующее. Далее будут приведены части файла:

```C
#ifndef	_STDIO_H_
#define	_STDIO_H_
// ...
#endif
```

`ifndef` / `endif` – блок условного оператора макропроцессора. Код между ними
будет исполнен (не вырезан макропроцессором), если макроопределение/макрос определено.

С помощью команды `define` можно определить макрос.

В данном случае конструкция, которая обрамляет весь файл – защита от повторного
определения функций и прочего при следующем `include`.

Также в заголовочных файлах можно проключать другие.

```C
#include <sys/cdefs.h>
#include <Availability.h>
```

Определять структуры

```C
struct __sbuf {
	unsigned char	*_base;
	int		_size;
};
```

TODO: Про extern рассказывать долго, но надо

```C
extern FILE *__stdinp;
extern FILE *__stdoutp;
extern FILE *__stderrp;
```

Объявлять "константы" – макросы, которые будут подставлены в код:

```C
#define	_IOFBF	0		/* setvbuf should set fully buffered */
#define	_IOLBF	1		/* setvbuf should set line buffered */
#define	_IONBF	2		/* setvbuf should set unbuffered */
```

Мы уже видели подобную "константу", когда читали входной поток и останавливались
по EOF.

Но не останавливаемся: тут есть ещё прототипы функций, которые мы сможем использовать:

```C
void	 clearerr(FILE *);
int	 fclose(FILE *);
int	 feof(FILE *);
int	 ferror(FILE *);
int	 fflush(FILE *);
int	 fgetc(FILE *);
int	 fgetpos(FILE * __restrict, fpos_t *);
char	*fgets(char * __restrict, int, FILE *);
```

Этот кусок кода примечателен тем, что макрос может быть не только каким-то
значением, но и "функцией" – не в СИшном виде, но по смыслу:

```C
#define	__sgetc(p) (--(p)->_r < 0 ? __srget(p) : (int)(*(p)->_p++))
#if defined(__GNUC__) && defined(__STDC__)
__header_always_inline int __sputc(int _c, FILE *_p) {
	if (--_p->_w >= 0 || (_p->_w >= _p->_lbfsize && (char)_c != '\n'))
		return (*_p->_p++ = _c);
	else
		return (__swbuf(_c, _p));
}
#else
/*
 * This has been tuned to generate reasonable code on the vax using pcc.
 */
#define	__sputc(c, p) \
	(--(p)->_w < 0 ? \
		(p)->_w >= (p)->_lbfsize ? \
			(*(p)->_p = (c)), *(p)->_p != '\n' ? \
				(int)*(p)->_p++ : \
				__swbuf('\n', p) : \
			__swbuf((int)(c), p) : \
		(*(p)->_p = (c), (int)*(p)->_p++))
#endif
//__
```

Ну и конец – ставим комменты о том, что закрываем, ибо можно запутаться:

```C
#endif /* _STDIO_H_ */
```

# Сделаем свою небольшую библиотеку

Вспомним связный список, который мы реализовали в теме про динамическую память.

Для начала определим "правила игры" / "контракт" нашей библиотеки в
заголовочном файле `linked_list.h`

```C
#ifndef	_LINKED_LIST_H_
#define	_LINKED_LIST_H_

#include <stdio.h>

typedef struct node {
  struct node * next;
  char val;
} node_t;

void print_list(node_t *head);
void add(node_t * head, char val);

#endif /* _LINKED_LIST_H_ */
```

Ну и сам код, "реализация контракта" `linked_list.c`:

```C
#include "linked_list.h"
#include <stdlib.h>

void print_list(node_t *head) {
  node_t * current = head;

  while (current != NULL) {
    printf("%c->", current->val);
    current = current->next;
  }

  printf("\n");
}

void add(node_t * head, char val) {
  node_t * current = head;
  while (current->next != NULL) {
    current = current->next;
  }

  current->next = (node_t * )malloc(sizeof(node_t));
  current->next->val = val;
  current->next->next = NULL;
}
```

И `main.c`, который будет использовать нашу библиотеку:

```C
#include "linked_list.h"

int main() {
  char c;
  node_t head = { .next = NULL, .val = ' ' };

  while ((c = getc(stdin)) != EOF) {
    add(&head, c);
  }

  print_list(&head);
}
```

Ну и начинаем собирать. Сначала объекты нашей библиотеки и основного файла:

```
→ clang -c linked_list.c -o linked_list.o
→ clang -c main.c -o main.o
```

В сущности, именно сейчас и происходит "компиляция". Компилятор берет файлы
с исходным кодом и получает из них объектные файлы.

Далее, собираем их в программу:

```
→ clang linked_list.o main.o -o linked_list_example
```

Линковщик берет объектные файлы и получает из них исполняемый файл. Сборка = компиляция + линковка.
