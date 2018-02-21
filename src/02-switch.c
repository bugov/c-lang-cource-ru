#include <stdio.h>

int main (void) {
  unsigned a = 1;
  char *text;  //* fix md formatting

  switch (a % 10) {
  case 1:
    text = "комментарий";
    break;
  case 2:
  case 3:
  case 4:
    text = "комментария";
    break;
  case 5: case 6: case 7: case 8: case 9: case 0:
    text = "комментариев";
  default:
    return 1;
  }

  puts(text);

  return 0;
}

