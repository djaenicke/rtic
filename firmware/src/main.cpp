#include "board.h"

int main(void)
{
  while (1)
  {
    toggleHeartbeat();
    delayMs(500u);
  }

  return 0;
}
