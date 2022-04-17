#ifndef IO_H
#define IO_H (1)

#include "gpio.h"
#include "stm32f4xx_hal.h"

#define HEARTBEAT_LED hal::Pin(hal::Port::C, 2u)

//! @brief Motor Driver 1 interface
#define D1_PWMA hal::PwmPin(hal::Pin(hal::Port::A, 2u), GPIO_AF3_TIM9, 1u)
#define D1_PWMB hal::PwmPin(hal::Pin(hal::Port::A, 3u), GPIO_AF3_TIM9, 2u)

#define D1_AIN1 hal::Pin(hal::Port::C, 3u)
#define D1_AIN2 hal::Pin(hal::Port::A, 4u)

#define D1_BIN1 hal::Pin(hal::Port::C, 4u)
#define D1_BIN2 hal::Pin(hal::Port::C, 5u)

#define D1_STBY hal::Pin(hal::Port::B, 0u)

//! @brief Motor Driver 2 interface
#define D2_PWMA hal::PwmPin(hal::Pin(hal::Port::B, 14u), GPIO_AF9_TIM12, 1u)
#define D2_PWMB hal::PwmPin(hal::Pin(hal::Port::B, 15u), GPIO_AF9_TIM12, 2u)

#define D2_AIN1 hal::Pin(hal::Port::B, 1u)
#define D2_AIN2 hal::Pin(hal::Port::B, 2u)

#define D2_BIN1 hal::Pin(hal::Port::C, 9u)
#define D2_BIN2 hal::Pin(hal::Port::C, 8u)

#define D2_STBY hal::Pin(hal::Port::A, 10u)

#define IMU_INT_PIN GPIO_PIN_13
#define IMU_INT_GPIO_PORT GPIOC

#define HSE_IN_PIN GPIO_PIN_0
#define HSE_IN_GPIO_PORT GPIOH

#define HSE_OUT_PIN GPIO_PIN_1
#define HSE_OUT_GPIO_PORT GPIOH

#define D1_MOTORA_CHA_PIN GPIO_PIN_0
#define D1_MOTORA_CHA_GPIO_PORT GPIOA

#define D1_MOTORA_CHB_PIN GPIO_PIN_1
#define D1_MOTORA_CHB_GPIO_PORT GPIOA

#define VBATT_AIN_PIN GPIO_PIN_5
#define VBATT_AIN_GPIO_PORT GPIOA

#define D1_MOTORB_CHA_PIN GPIO_PIN_6
#define D1_MOTORB_CHA_GPIO_PORT GPIOA

#define D1_MOTORB_CHB_PIN GPIO_PIN_7
#define D1_MOTORB_CHB_GPIO_PORT GPIOA

#define CAN2_RS_PIN GPIO_PIN_10
#define CAN2_RS_GPIO_PORT GPIOB

#define D2_MOTORA_CHA_PIN GPIO_PIN_6
#define D2_MOTORA_CHA_GPIO_PORT GPIOC

#define D2_MOTORA_CHB_PIN GPIO_PIN_7
#define D2_MOTORA_CHB_GPIO_PORT GPIOC

#define D2_MOTORB_CHA_PIN GPIO_PIN_8
#define D2_MOTORB_CHA_GPIO_PORT GPIOA

#define D2_MOTORB_CHB_PIN GPIO_PIN_9
#define D2_MOTORB_CHB_GPIO_PORT GPIOA

#define D2_STBY_PIN GPIO_PIN_10
#define D2_STBY_GPIO_PORT GPIOA

#define RF_SCK_PIN GPIO_PIN_10
#define RF_SCK_GPIO_PORT GPIOC

#define RF_MISO_PIN GPIO_PIN_11
#define RF_MISO_GPIO_PORT GPIOC

#define RF_MOSI_PIN GPIO_PIN_12
#define RF_MOSI_GPIO_PORT GPIOC

#define RF_CS_PIN GPIO_PIN_2
#define RF_CS_GPIO_PORT GPIOD

#define RF_IRQ_PIN GPIO_PIN_4
#define RF_IRQ_GPIO_PORT GPIOB

#define RF_EN_PIN GPIO_PIN_5
#define RF_EN_GPIO_PORT GPIOB

#define DEBUG_TX_PIN GPIO_PIN_6
#define DEBUG_TX_GPIO_PORT GPIOB

#define DEBUG_RX_PIN GPIO_PIN_7
#define DEBUG_RX_GPIO_PORT GPIOB

#define IMU_SCL_PIN GPIO_PIN_8
#define IMU_SCL_GPIO_PORT GPIOB

#define IMU_SDA_PIN GPIO_PIN_9
#define IMU_SDA_GPIO_PORT GPIOB

#endif  // IO_H
