#ifndef NRF_24_HAL_H
#define NRF_24_HAL_H

#include <stdint.h>

#include "rf_interface.h"

#define nRF24_CSN_L enableNrf24ChipSelect()
#define nRF24_CSN_H disableNrf24ChipSelect()
#define nRF24_LL_RW readWriteNrf24

uint8_t nRF24_LL_RW(uint8_t data);

#endif  // NRF_24_HAL_H
