#ifndef RF_MESSAGES_H
#define RF_MESSAGES_H (1)

#include <stdint.h>

typedef struct __attribute__((packed))
{
  uint8_t id;
  uint8_t version;
  uint8_t tx_cnt;
} RfMessageHeader;

typedef struct __attribute__((packed))
{
  uint16_t x_counts;
  uint16_t y_counts;
  uint8_t z_press_cnt;
} JoystickInput;

typedef struct __attribute__((packed))
{
  RfMessageHeader header;
  JoystickInput l_joystick;
  JoystickInput r_joystick;
} UserInputMessage;

#endif  // RF_MESSAGES_H
