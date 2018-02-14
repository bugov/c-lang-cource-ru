#include "stdio.h"

int main(void) {
    int a = 1;              // 1
    int b = 2;              // 2
    int c = a / b;          // ?
    printf("%d\n", c);
    
    float d = a / 2.0;      // ?
    printf("%f\n", d);

    d = a / 2;              // !
    printf("%f\n", d);
    
    return 0;
}
