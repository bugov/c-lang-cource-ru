# Модули Python на Си

1. Можно делать свои "встроенные" типы объектов,
2. Можно использовать Си-библиотеки, системные вызовы.
3. Нет ограничения в виде GIL.

## hello.c

```C
#include <Python.h>

static PyObject *print_hello(
  PyObject *self,
  PyObject *args  // Аргументы функции
) {
  const char *name;
  
  // Извлекаем агрументы
  if (! PyArg_ParseTuple(args, "s", &name)) {
    return NULL;
  }
  
  printf("Hello, %s\n", name);
  
  // None - глобальная переменная, мы просто ей воспользуемся.
  // Добавим счётчик ссылок на "питоний объект".
  Py_INCREF(Py_None);
  return Py_None;
}

// Определяем методы модуля
static PyMethodDef methods[] = {
    // Наша функция
    { "hello", print_hello, METH_VARARGS, "prints hello" },
    
    // Последняя "функция" - где надо остановиться.
    // Этакий стоп-байт.
    { NULL, NULL, 0, NULL },
};

// Определяем структуру модуля
static struct PyModuleDef hello_def = {
	PyModuleDef_HEAD_INIT,
	"hello",  // Имя модуля,
	NULL,     // Документация модуля
	-1,       // Размер стейта модуля. -1 - стейт в глобальных переменных
	methods,
};

PyMODINIT_FUNC PyInit_hello(void) {
    return PyModule_Create(&hello_def);
}
```

```Bash
sudo apt install python3-dev
clang \
    -fPIC \
    --shared \
    hello.c \
    -I /usr/include/python3.6/ \
    -o hello.so
```

```Python
$ python3
Python 3.6.5 (default, Apr  1 2018, 05:46:30) 
[GCC 7.3.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> from hello import hello
>>> hello("Gosha")
Hello, Gosha
>>> hello()
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: function takes exactly 1 argument (0 given)
```



