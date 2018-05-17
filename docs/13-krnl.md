# Условия "жизни" в ядре

Вы ломаете всё и не факт что сразу.

Ваш код состоит из:

1. Зарузка
2. Выгрузка
3. Обработчики событий

Хотя ядро предоставляет некоторые функции вроде printk (которая служит заменой printf)
и kmalloc (работает похоже на malloc), в основном вы остаётесь наедине с железом.
Вдобавок, после выгрузки модуля следует полностью почистить за собой

Идём на https://mirrors.edge.kernel.org/pub/linux/kernel/v4.x/ и копируем ссылку
на uname -r версию.

Ставим необходимое:

```
apt install linux-headers-<version>-common
```

Простейший модуль:

```C
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("WTFPL");
MODULE_AUTHOR("Gosha");
MODULE_DESCRIPTION("Hello, kernel!");
MODULE_VERSION("1.00");

static int __init mod_init(void) {
  printk(KERN_INFO “Hello, Kernel!\n”);
  return 0;
}

static void __exit mod_exit(void) {
  printk(KERN_INFO “Goodbye, Kernel!\n”);
}

module_init(mod_init);
module_exit(mod_exit);
```

Makefile

```C
obj-m += mod.c

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

obj-m – это список модулей ядра для сборки. .o будет автоматически
собран из соответствующего .c файла (нет необходимости указывать
список исходных файлов)
