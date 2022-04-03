#include <stdio.h>
#include "board.h"

void main(void)
{
  initBoard();

  serialTxString("\"initBoard\" complete.\r\n");

  while(1)
  {
    toggleHeartbeat();
    delayMs(500u);
  }
}
