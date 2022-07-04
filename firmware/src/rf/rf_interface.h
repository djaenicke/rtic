#ifndef RF_INTERFACE_H
#define RF_INTERFACE_H (1)

#include <stdint.h>

void threadRfInterface(const void* argument);

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
