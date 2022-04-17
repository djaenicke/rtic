#include "board.h"

int main(void)
{
  initBoard();

  serialTxString("\"initBoard\" complete.\r\n");

  while (1)
  {
    toggleHeartbeat();
    delayMs(500u);
  }

  return 0;
}
