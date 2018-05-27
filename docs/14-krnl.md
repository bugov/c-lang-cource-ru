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
на `uname -r` версию. Либо же

Ставим необходимое:

```
apt install linux-headers-<version>-generic
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
  printk(KERN_INFO "Hello, Kernel!\n");
  return 0;
}

static void __exit mod_exit(void) {
  printk(KERN_INFO "Goodbye, Kernel!\n");
}

module_init(mod_init);
module_exit(mod_exit);
```

Makefile

```C
obj-m += mod.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

obj-m – это список модулей ядра для сборки. .o будет автоматически
собран из соответствующего .c файла (нет необходимости указывать
список исходных файлов)

```Bash
sudo insmod mod.ko
sudo dmesg
```

И выход:

```Bash
sudo rmmod mod
sudo dmesg
```

## Пишем свой обработчик прерываний

Рассмотрим пример chardev из [гайда по разработке модулей ядра](http://www.tldp.org/LDP/lkmpg/2.6/html/).

```C
#include <linux/module.h>
#include "usr/src/linux-headers-4.15.0-20/include/linux/configfs.h"
#include <linux/init.h>
#include <linux/tty.h>    /* определение fg_console, MAX_NR_CONSOLES */
#include <linux/kd.h>     /* определение KDSETLED */
#include <linux/console_struct.h> /* определение  vc_cons */

MODULE_DESCRIPTION("Пример module illustrating the use of Keyboard LEDs.");
MODULE_AUTHOR("Daniele Paolo Scarpazza");
MODULE_LICENSE("GPL");

struct timer_list my_timer;
struct tty_driver *my_driver;
char kbledstatus = 0;

#define BLINK_DELAY   HZ/5
#define ALL_LEDS_ON   0x07
#define RESTORE_LEDS  0xFF

/*
 * Функция my_timer_func мигает индикаторами на клавиатуре периодически вызывая 
 * ioctl() драйвера клавиатуры с командой KDSETLED. Дополнительную информацию,
 * по командам ioctl виртуального терминала, вы найдете в:
 *     /usr/src/linux/drivers/char/vt_ioctl.c, function vt_ioctl().
 *
 * Дополнительный аргумент команды KDSETLED -- значение 7 (перевод в режим
 * LED_SHOW_IOCTL -- управление индикаторами через ioctl), значение 0xFF --
 * (любое значение, большее 7, перевод в режим LED_SHOW_FLAGS --
 * отображение фактического состояния клавиатуры). Дополнительная информация:
 *     /usr/src/linux/drivers/char/keyboard.c, function setledstate().
 * 
 */

static void my_timer_func(unsigned long ptr)
{
  int *pstatus = (int *)ptr;

  if (*pstatus == ALL_LEDS_ON)
    *pstatus = RESTORE_LEDS;
  else
    *pstatus = ALL_LEDS_ON;

  (my_driver->ioctl) (vc_cons[fg_console].d->vc_tty, NULL, KDSETLED,
          *pstatus);

  my_timer.expires = jiffies + BLINK_DELAY;
  add_timer(&my_timer);
}

static int __init kbleds_init(void)
{
  int i;

  printk(KERN_INFO "kbleds: loading\n");
  printk(KERN_INFO "kbleds: fgconsole is %x\n", fg_console);
  for (i = 0; i < MAX_NR_CONSOLES; i++) {
    if (!vc_cons[i].d)
      break;
    printk(KERN_INFO "poet_atkm: console[%i/%i] #%i, tty %lx\n", i,
           MAX_NR_CONSOLES, vc_cons[i].d->vc_num,
           (unsigned long)vc_cons[i].d->vc_tty);
  }
  printk(KERN_INFO "kbleds: finished scanning consoles\n");

  my_driver = vc_cons[fg_console].d->vc_tty->driver;
  printk(KERN_INFO "kbleds: tty driver magic %x\n", my_driver->magic);

  /*
   * Инициировать таймер
   */
  init_timer(&my_timer);
  my_timer.function = my_timer_func;
  my_timer.data = (unsigned long)&kbledstatus;
  my_timer.expires = jiffies + BLINK_DELAY;
  add_timer(&my_timer);

  return 0;
}

static void __exit kbleds_cleanup(void)
{
  printk(KERN_INFO "kbleds: unloading...\n");
  del_timer(&my_timer);
  (my_driver->ioctl) (vc_cons[fg_console].d->vc_tty, NULL, KDSETLED,
          RESTORE_LEDS);
}

module_init(kbleds_init);
module_exit(kbleds_cleanup);
```


