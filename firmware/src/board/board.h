#ifndef BOARD_H
#define BOARD_H (1)

#include <stdint.h>

#include "io.h"
#include "stm32f4xx_hal.h"

#ifdef __cplusplus
 extern "C" {
#endif
void initBoard(void);
void serialTxString(const char* const string);
void toggleHeartbeat(void);
void delayMs(const uint32_t delay_ms);
#ifdef __cplusplus
}
#endif

#endif  // BOARD_H
