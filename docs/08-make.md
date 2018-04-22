# Автоматизация работы с помощью make

Когда-то давно люди решили автоматизировать операции,
связанные со сборкой и поддержкой кода. Решением стала команда make.
Она делала одну вещь и хорошо.

Со временем эта "одна вещь" стала больше,
а make продолжал стараться делать её хорошо, но потерял простоту и низкий
порог входа. Проще нажать на кнопочку в IDE, чем прочитать `man make`,
который даже не даст вам ответа на вопрос "а что писать в Makefile".

Однако, полезно понимать, как происходит сборка, да и сам механизм makefile-ов
может вам помочь ещё ни раз – даже в тех же сборках js-пакетов. Ну и в целом –
это часть той древней первозданной "магии", которую приятно понимать.

Для начала в общем виде:

```
цель: зависимости
[tab] команда
```

Стоит заметить, что используется табуляция, а не пробельные символы.
Часто можно увидеть, что "правильный" Makefile почему-то не собирается.
Советую включить в редаторе отображение "невидимых" символов.

И пример из прошлой темы, где мы писали библиотеку для связных списков:

```
all: main.c linked_list.c
	clang linked_list.c main.c -o linked_list_example
```

Далее сама сборка с помощью make

```
→ make all
```

Или же просто

```
→ make
```

– цель `all` – цель по умолчанию.

Могли ли мы это же сделать в предыдущем случае (не разделять компиляцию и линковку)?
Естественно, но зачем-то мы собирали отдельно? Если мы вносим изменения в код,
нам, естественно, нужно будет перекомпилировать и слинковать. Но если мы
разделяем компиляцию/трансляцию по файлам/библиотекам, то надо перекомпилировать
только этот файл. Это ускоряет сборку, особенно если у нас большой проект.
И да, это называется "инкрементальная компиляция".

Теперь напишем Makefile с использованием инкрементальной компиляции:

```
linked_list.o: linked_list.c
	clang -c linked_list.c -o linked_list.o
main.o: main.c
	clang -c main.c -o main.o
all: main.o linked_list.o
	clang linked_list.o main.o -o linked_list_example
```

– теперь будут компилироваться только изменённые файлы.

Также, возможно вы видели и цель `install`, когда устанавливали программы:

```
→ make
→ make install
```

а вот файла install там не было. Становится очевидно, что целями могут быть не
только файлы, но и "фиктивные цели". Они определяются "цели" `.PHONY`:

```
.PHONY: all clean install uninstall

linked_list.o: linked_list.c
	clang -c linked_list.c -o linked_list.o
main.o: main.c
	clang -c main.c -o main.o

all: main.o linked_list.o
	clang linked_list.o main.o -o linked_list_example
clean:
	rm -rf hello *.o
install:
	install ./linked_list_example /usr/local/bin
uninstall:
	rm -rf /usr/local/bin/linked_list_example
```

– clean, install и uninstall – частые там гости.

Также make позволяет использовать переменные:

```
СС=clang
CFLAGS=-Wall
LDFLAGS=

.PHONY: all clean install uninstall

linked_list.o: linked_list.c
	$(CC) $(CFLAGS) -c linked_list.c -o linked_list.o
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o
all: main.o linked_list.o
	$(CC) $(LDFLAGS) linked_list.o main.o -o linked_list_example

clean:
	rm -rf hello *.o
install:
	install ./linked_list_example /usr/local/bin
uninstall:
	rm -rf /usr/local/bin/linked_list_example
```

На этом мы завершим знакомство с make. Так базово мы можем автоматизировать сборку,
однако, сам make на этом только начинается: помимо банальных спец-переменных,
там ещё целый язык программирования, который помогает автоматизировать
автоматизацию.
