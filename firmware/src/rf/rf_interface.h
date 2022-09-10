#ifndef RF_INTERFACE_H
#define RF_INTERFACE_H (1)

#include <stdbool.h>
#include <stdint.h>

#include "rf_messages.h"

void threadRfInterface(const void* argument);

void initUiMessageBuffer(void);
bool userInputMessageBufferRead(UserInputMessage* const ui_msg);

#ifdef __cplusplus
extern "C"
{
#endif
  void enableNrf24ChipSelect(void);
  void disableNrf24ChipSelect(void);
#ifdef __cplusplus
}
#endif

#endif  // RF_INTERFACE_H
