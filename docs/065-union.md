## Union

Объединения в Си похожи на структуры, однако поля, объявленные в них, имеют
начало на одном и том же адресе. То есть это возможность манипулировать одними
и теми же данными с помошью разных их представлений.

Например, с float и int:

```C
#include <stdio.h>

union floatint{
  float f;
  int i;
};

int main() {
  // Устанавливаем значение через представление "f" (float)
  union floatint u = { 10.f };

  printf("%f\n", u.f);
  printf("%x\n", u.i);

  return 0;
}
```

Это довольно занятно, но интереснее делать объединения структур!
Так мы можем работать с одной и той-же памятью, но в разных её интерпретациях.
Например, мы можем попытаться работать с 32-мя битами как с:

1. 4-мя байтами,
2. 2-мя машинными словами,
3. 1-им двойным машинным словом.

```C
#include <stdio.h>

typedef union Register32 {
    struct {
        unsigned char byte1;
        unsigned char byte2;
        unsigned char byte3;
        unsigned char byte4;
    } bytes;
    struct {
        unsigned short low;
        unsigned short high;
    } words;
    unsigned dword;
} Register32;

int main() {
    Register32 reg;
    reg.dword = 0x0000C0FF;

    printf("    dword \t%08x\n", reg.dword);
    printf(" low word \t%04x\n", reg.words.low);
    printf("high word \t%04x\n", reg.words.high);
    printf("    byte1 \t%02x\n", reg.bytes.byte1);  
    printf("    byte2 \t%02x\n", reg.bytes.byte2);  
    printf("    byte3 \t%02x\n", reg.bytes.byte3);  
    printf("    byte4 \t%02x\n", reg.bytes.byte4);  

    return 0;
}
```

TODO: добавить пример по работе с TCP/UDP заголовками через объединение.

## ДЗ

Написать на C API функцию Python3, получающую по объекту его счётчик ссылок.
