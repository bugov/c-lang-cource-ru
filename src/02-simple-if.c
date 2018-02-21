#include <stdio.h>

int main(void) {
    int a, b;
    scanf("%d %d", &a, &b);
    
    if (b == 0) {
        return 1;
    }
    else {
        printf("%d\n", a / b);
    }

    return 0;
}

