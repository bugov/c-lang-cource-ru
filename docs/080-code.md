# Примеры решений ДЗ

Примеры решений домашних заданий, часть 2.

## Функциональная пара

Написать функции filter, reduce - аналоги [функций
python](https://ru.wikipedia.org/wiki/Функциональное_программирование_на_Python).

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Функция фильтрации
 *
 * Отфильтровывает из входящего массива элементы,
 * проходящие по критериям фильтрующей функции.
 * Результат складывает в результирующий массив.
 *
 * Возвращает количество елементов, прошедших фильтрацию.
 * Вопрос: какой тип стоит здесь использовать вместо int?
 */
int filter(
    void *from,
    void *to,
    size_t count,
    size_t size,
    int (*where)(void *)
) {
    // Для использования арифметики указателей,
    // преобразуем в указатели на байты.
    char *from_bytes = (char *)from;
    char *to_bytes = (char *)to;

    int index = 0;
    for (size_t i = 0; i < count; i++) {
        if (where(from_bytes + size * i)) {
            // Копируем данные из исходного в целевой массив.
            // Вопрос: как это ещё можно было сделать?
            memcpy(to_bytes + size * index, from_bytes + size * i, size);
            index++;
        }
    }
    return index;
}

/**
 * Функция свёртки (левой)
 *
 * Первый шаг - положить в результат
 * первый элемент входного массива.
 * Далее - идём слева направо и применяем операцию
 * к результату и элементу массива и складываем в результат.
 *
 * Вопрос: что делать в случае пустого входного массива?
 */
void reduce(
    void *array,
    void *result,
    size_t count,
    size_t size,
    void (*consumer)(void *, void *)
) {
    if (size == 0) {
        return;
    }

    char *array_bytes = (char *)array;
    char *result_bytes = (char *)result;

    memcpy(result_bytes, array_bytes, size);

    for (int i = 1; i < count; i++) {
        consumer(result_bytes, array_bytes + size * i);
    }
}

/**
 * Проверка на чётность int-а
 */
int is_odd(int* a) {
    return (*a) % 2 == 0;
}

/**
 * Сложение int-ов
 */
void sum(int *n_1, int *n_2) {
    *n_1 = *n_1 + *n_2;
}

/**
 * Демонстрация работы
 */
int main(void) {
    int a[6] = {1, 1, 2, 3, 5, 8};
    int b[6];

    int size = filter(a, b, sizeof(a) / sizeof(int), sizeof(int), is_odd);

    printf("Фильтр: ");
    for (int i = 0; i < size; ++i) {
        printf("%d ", b[i]);
    }
    printf("\n");

    int c;
    reduce(a, &c, sizeof(a) / sizeof(int), sizeof(int), sum);

    printf("Свёртка: %d\n", c);

    return 0;
}
```

## Структурированная пара

Написать на C API функцию Python3, получающую по объекту его счётчик ссылок.

**Зная, что у меня замечательные студенты, которые умеют гуглить,
решил дать им это задание. Пусть и тема о заголовочных файлах впереди
– так получат хоть "шапочное" знакомство. А написать свой модуль на Си –
знаю, руки чешутся!**

`src/counter.c`:

```C
/**
 * Работаем с Python - его и подключаем.
 * Понадобится пакет python3-dev (может отличаться)
 */
#include <Python.h>

/**
 * Требуемая функция - получает 2 Python-объекта:
 * self и аргументы функции. Нас интересует только 2-ой.
 *
 * Вернёт также Python-объект (у нас же всё "объект").
 * Само собой, объектов не существует, это Сишные структуры.
 *
 * Оставляю за вами возможность рассмотреть структуру PyObject
 * - она не сложная, лежит в Python.h
 */
static PyObject* ref_count(PyObject* self, PyObject* args) {
    PyObject* object;

    /**
     * Документацию по PyArg_ParseTuple можно посмотреть здесь
     * https://docs.python.org/3/c-api/arg.html
     *
     * args - структура, которую мы парсим
     * "O" - парсим в объект
     * Указатель на object - куда мы это положим
     *
     * Если мы получили то, что не является объектом – NULL
     */
    if (! PyArg_ParseTuple(args, "O", &object)) {
        return NULL;
    }

    /**
     * По ссылке выше также можно отыскать описание Py_BuildValue.
     * Собираем из байтов указатель на PyObject.
     *
     * "i" - это наш int. Именно его мы получим благодаря функции
     * Py_REFCNT, который получает из PyObject счётчик ссылок.
     *
     * Доп. задание - не использовать Py_REFCNT для этой функции.
     */
    return Py_BuildValue("i", Py_REFCNT(object));
}

// Python-овский docstring - документация функции
static char ref_docs[] = "count(): get reference count";

// Описание метода. Да, тоже структура.
static PyMethodDef ref_funcs[] = {
    {
        "count",                  // Название "метода"
        (PyCFunction)ref_count,   // Сама функция
        METH_VARARGS,             // Тип параметров
        ref_docs                  // Документация функции
    },
    // "Нуль-терминатор" - функций может быть много,
    // надо знать, когда останоиться.
    {NULL, NULL, 0, NULL}
};

// Описание модуля
static struct PyModuleDef ref_def = {
    PyModuleDef_HEAD_INIT,  // Стандартная инициализация модуля
    "ref",                  // Название модуля
    NULL,                   // Документация - обойдёмся без неё
    -1,                     // Размер – нам не нужно
    ref_funcs,              // Методы модуля
};

// Сама функция инициализации модуля
PyMODINIT_FUNC PyInit_ref(void) {
    return PyModule_Create(&ref_def);
}
```

Всё это можно собрать в Python модуль с помощью следующего `setup.py`:

```Python3
import os
from setuptools import setup, Extension

setup(
    name='ref',
    version='0.0.1',
    zip_safe=False,
    include_package_data=True,

    ext_modules=[
        Extension(
            'ref',
            ['src/counter.c'],
        )
    ],
)
```

Ну и `python3 setup.py install` для установки.
