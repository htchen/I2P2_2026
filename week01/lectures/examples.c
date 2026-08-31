#include <stdio.h>

int main(void) {
  int value;
  int sum = 0;

  while (scanf("%d", &value) == 1) {
    if (value > 0 && value % 2 == 0) {
      sum += value;
    }
  }

  printf("positive even sum = %d\n", sum);
  return 0;
}
