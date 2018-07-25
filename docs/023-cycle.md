## Циклы

Одна из задач компьютеров после обсчёта ядерных взрывов – автоматизация
рутинных повторяющихся операций. А если мы говорим о повторении, то самое время
поговорить о циклах.

### while

В общем виде while выглядит так:

```C
while (выражение1)
    оператор1
```

Если выражение1 истинно, то выполняем оператор1, пока выражение1 не станет
ложным, либо пока нас не остановят оператором break.

```C
unsigned x = 0;  // unsigned int
unsigned y = 0;  // беззнаковое целочисленное

while (x <= y) {
    x = y;
    y++;
}
```

Когда остановится цикл? Правильно, когда y переполнится. В x будет лежать
самое большое unsigned число.

### for

Также известен как "Сишный for":

```C
for (выражение1; выражение2; выражение3)
    оператор1;
```

Выражение1 выполняется при старте работы цикла.

Выражение2 – условия остановки цикла.

Выражение3 выполняется каждыую итерацию цикла.

Стандартный пример – обход элементов массива, но это будет в следующей теме.
Сейчас же – посчитаем факториал:

```C
unsigned fact = 1;
for (int iter = 1; iter <= 5; iter++) {
    fact = fact * iter;
}
```

Мы можем также сделать это через while:

```C
unsigned fact = 1;
int iter = 1;

while (iter <= 5) {
    fact = fact * iter;
    iter++;
}
```

А предыдущий пример мы можем записать на for вместо while:

```C
unsigned x = 0;
unsigned y = 0;

for (; x < y; y++) {
    x = y;
}
```

### do-while

```C
do
    оператор1;
while (выражение1);
```

В отличии от `while`, `do-while` выполняет 1 раз оператор1, далее – проверяет
выражение1 и, если оно истинно, снова выполняет оператор1, пока выражение1
не станет ложным.

Имеет смысл при операциях, которые нужно повторять, если случилась ошибка.
Например, curl-запрос может не выполниться с первого раза – сначала делаем,
потом проверяем на наличие ошибки и повторяем в случае её.

### break и continue

Оператор `break` мы уже видели – он прерывает работу цикла или `switch`,
выполнение продолжится с оператора, следующего за циклом или `switch`.

`continue` также вмешивается в работу цикла, но не прерывает его полностью –
он переводит к следющей итерации цикла.

```C
for (int i = 0; i < 100; i++) {
    if (i % 2) {
        continue;
    }

    printf("%d\n", i);
}
```

Если `i` нечётное – пропускаем. Если чётное – продолжаем, то есть печатаем.

## ДЗ

Написать утилиту, позволяющую считать статистику по символам: цифры, латиница.

Пример работы:

```Bash
→ cat /etc/passwd | ./counter
Digits: 12
Latin symbols: 112
```

Подсказка. Читать символы можно функцией `getc` из `stdio`, читать нужно из `stdin`,
а символ конца ввода – `EOF`.

```C
while ((c = getc(stdin)) != EOF)
```