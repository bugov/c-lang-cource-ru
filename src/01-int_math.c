#include "stdio.h"

int main(void) {
    int a = 1;      // 1
    
    a += 1;         // 2
    printf("%d\n", a);
    
    a = a * 2;      // 4
    printf("%d\n", a);
    
    a /= 2;         // 2
    printf("%d\n", a);
    
    int b = a % 3;  // 2
    printf("%d\n", b);  
}

