#include "board.h"
#include "digital_out.h"
#include "io.h"

hal::DigitalOut heartbeat(HEARTBEAT_LED);

int main(void)
{
  while (1)
  {
    heartbeat.toggle();
    delayMs(500u);
  }

  return 0;
}
