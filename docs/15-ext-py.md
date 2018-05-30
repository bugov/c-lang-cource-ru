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
  // Добавим ссылок счётчик на "питоний объект".
  // Или можно использовать макрос Py_RETURN_NONE
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

## 

```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>
#include <linux/netfilter.h>
#include <libnetfilter_queue/libnetfilter_queue.h>

static u_int32_t print_pkt (struct nfq_data *tb) {
  struct nfqnl_msg_packet_hdr *ph;
  
  int id;
  u_int32_t ifi;
  int ret;
  unsigned char *data;

  ph = nfq_get_msg_packet_hdr(tb);
  if (ph) {
    id = ntohl(ph->packet_id);
    printf("hw_protocol=0x%04x hook=%u id=%u ",
      ntohs(ph->hw_protocol), ph->hook, id);
  }
  
  ifi = nfq_get_indev(tb);
  if (ifi) {
    printf("indev=%u ", ifi);
  }
  
  ifi = nfq_get_outdev(tb);
  if (ifi) {
    printf("outdev=%u ", ifi);
  }
  
  ret = nfq_get_payload(tb, &data);
  if (ret >= 0) {
    printf("payload_len=%d ", ret);
  }
  printf("\n");
  return id;
}

static int cb(
  struct nfq_q_handle *qh,
  struct nfgenmsg *nfmsg,
  struct nfq_data *nfa,
  void *data
) {
  u_int32_t id = print_pkt(nfa);
  struct nfqnl_msg_packet_hdr *ph;
  ph = nfq_get_msg_packet_hdr(nfa);
  return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
}

int main(int argc, char **argv) {
  struct nfq_handle *h;
  struct nfq_q_handle *qh;
  int fd;
  int rv;
  char buf[4096];

  h = nfq_open();
  if (!h) {
    perror("error during nfq_open()");
    exit(1);
  }

  if (nfq_unbind_pf(h, AF_INET) < 0) {
    perror("error during nfq_unbind_pf()");
    exit(1);
  }

  if (nfq_bind_pf(h, AF_INET) < 0) {
    perror("error during nfq_bind_pf()");
    exit(1);
  }

  qh = nfq_create_queue(h, 1, &cb, NULL);
  if (!qh) {
    perror("error during nfq_create_queue()");
    exit(1);
  }

  if (nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff) < 0) {
    perror("can't set packet_copy mode");
    exit(1);
  }

  fd = nfq_fd(h);

  while ((rv = recv(fd, buf, sizeof(buf), 0))) {
    nfq_handle_packet(h, buf, rv);
  }

  nfq_destroy_queue(qh);
  nfq_close(h);

  exit(0);
}
```

```Bash
sudo apt install libnetfilter-queue1 libnetfilter-queue-dev
```

```Bash
clang -lnetfilter_queue 1.c -o sniffer
```

```Bash
sudo iptables \
  -A INPUT \
  -d 127.127.127.127 \
  -j NFQUEUE --queue-num 1

ping 127.127.127.127
```

```Bash
sudo ./sniffer
```

................

```Bash
clang \
  -fPIC \
  --shared \
  -lnetfilter_queue \
  -I /usr/include/python3.6/ \
  nfq.c \
  -o nfq.so
```

```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>
#include <linux/netfilter.h>
#include <libnetfilter_queue/libnetfilter_queue.h>
#include <Python.h>

#if PY_MAJOR_VERSION >= 3
  #define PyInt_FromLong PyLong_FromLong
#endif

struct ret_data {
  int pkt_id;
  u_int32_t indev;
  u_int32_t outdev;
};

static struct nfq_handle *h;
static struct nfq_q_handle *qh;
static struct ret_data ret;


static u_int32_t print_pkt (struct nfq_data *tb) {
  struct nfqnl_msg_packet_hdr *ph;
  
  int id;
  u_int32_t ifi;
  int result;
  unsigned char *data;

  ph = nfq_get_msg_packet_hdr(tb);
  if (ph) {
    id = ntohl(ph->packet_id);
    printf("hw_protocol=0x%04x hook=%u id=%u ",
      ntohs(ph->hw_protocol), ph->hook, id);
  }
  
  ret.indev = nfq_get_indev(tb);
  ret.outdev = nfq_get_outdev(tb);

  result = nfq_get_payload(tb, &data);
  if (result >= 0) {
    printf("payload_len=%d ", result);
  }
  printf("\n");
  ret.pkt_id = id;
  return id;
}

static int cb(
  struct nfq_q_handle *qh,
  struct nfgenmsg *nfmsg,
  struct nfq_data *nfa,
  void *data
) {
  u_int32_t id = print_pkt(nfa);
  struct nfqnl_msg_packet_hdr *ph;
  ph = nfq_get_msg_packet_hdr(nfa);
  return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
}

static PyObject * open_queue_fd(
  PyObject *self,
  PyObject *args
) {
  unsigned queue_num;
  if (! PyArg_ParseTuple(args, "K", &queue_num)) {
    return NULL;
  }

  int fd;
  
  h = nfq_open();
  if (!h) {
    perror("error during nfq_open()");
    exit(1);
  }

  if (nfq_bind_pf(h, AF_INET) < 0) {
    perror("error during nfq_bind_pf()");
    exit(1);
  }

  qh = nfq_create_queue(h, queue_num, &cb, NULL);
  if (!qh) {
    perror("error during nfq_create_queue()");
    exit(1);
  }

  if (nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff) < 0) {
    perror("can't set packet_copy mode");
    exit(1);
  }

  fd = nfq_fd(h);
  return PyInt_FromLong(fd);
}

static PyObject * get_pkt(
  PyObject *self,
  PyObject *args
) {
  char buf[4096];
  int fd;
  
  if (! PyArg_ParseTuple(args, "L", &fd)) {
    return NULL;
  }

  int rv = recv(fd, buf, sizeof(buf), 0);
  if (rv == 0) {
    Py_INCREF(Py_None);
    return Py_None;
  }
  
  nfq_handle_packet(h, buf, rv);
  
  PyObject *tpl = PyTuple_New(3);
  PyTuple_SetItem(tpl, 0, ret.pkt_id);
  PyTuple_SetItem(tpl, 1, ret.indev);
  PyTuple_SetItem(tpl, 2, ret.outdev);
  
  return tpl;
}

static PyObject * close_queue_fd(
  PyObject *self,
  PyObject *args
) {
  nfq_destroy_queue(qh);
  nfq_close(h);
  
  Py_INCREF(Py_None);
  return Py_None;
}

static PyMethodDef methods[] = {
  { "open_queue_fd", open_queue_fd, METH_VARARGS, "Get nfqueue fd" },
  { "close_queue_fd", close_queue_fd, METH_VARARGS, "Close nfqueue fd" },
  { "get_pkt", get_pkt, METH_VARARGS, "Get packet" },
  { NULL, NULL, 0, NULL },
};

// Определяем структуру модуля
static struct PyModuleDef nfq_mod_def = {
	PyModuleDef_HEAD_INIT,
	"nfq",  // Имя модуля,
	NULL,     // Документация модуля
	-1,       // Размер стейта модуля. -1 - стейт в глобальных переменных
	methods,
};

PyMODINIT_FUNC PyInit_nfq(void) {
  return PyModule_Create(&nfq_mod_def);
}
```

