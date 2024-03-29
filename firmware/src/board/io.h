#ifndef IO_H
#define IO_H (1)

#include "gpio.h"
#include "stm32f4xx_hal.h"

#define HEARTBEAT_LED hal::Pin(hal::Port::C, 2u)

//! @brief Motor Driver 1 interface
#define D1_PWMA hal::PwmPin(hal::Pin(hal::Port::A, 2u), GPIO_AF3_TIM9, 1u)
#define D1_PWMB hal::PwmPin(hal::Pin(hal::Port::A, 3u), GPIO_AF3_TIM9, 2u)

// clang-format off
#define D1_MOTORA_CHA hal::GPIO(hal::Pin(hal::Port::A, 0U), hal::GpioMode::AF_PUSH_PULL, \
                                hal::GpioPullType::NO_PULL, GPIO_AF2_TIM5)
#define D1_MOTORA_CHB hal::GPIO(hal::Pin(hal::Port::A, 1U), hal::GpioMode::AF_PUSH_PULL, \
                                hal::GpioPullType::NO_PULL, GPIO_AF2_TIM5)
#define D1_MOTORB_CHA hal::GPIO(hal::Pin(hal::Port::A, 6U), hal::GpioMode::AF_PUSH_PULL, \
                                hal::GpioPullType::NO_PULL, GPIO_AF2_TIM3)
#define D1_MOTORB_CHB hal::GPIO(hal::Pin(hal::Port::A, 7U), hal::GpioMode::AF_PUSH_PULL, \
                                hal::GpioPullType::NO_PULL, GPIO_AF2_TIM3)
// clang-format on

#define D1_AIN1 hal::Pin(hal::Port::C, 3u)
#define D1_AIN2 hal::Pin(hal::Port::A, 4u)

#define D1_BIN1 hal::Pin(hal::Port::C, 4u)
#define D1_BIN2 hal::Pin(hal::Port::C, 5u)

#define D1_STBY hal::Pin(hal::Port::B, 0u)

//! @brief Motor Driver 2 interface
#define D2_PWMA hal::PwmPin(hal::Pin(hal::Port::B, 14u), GPIO_AF9_TIM12, 1u)
#define D2_PWMB hal::PwmPin(hal::Pin(hal::Port::B, 15u), GPIO_AF9_TIM12, 2u)

#define D2_MOTORA_CHA hal::Pin(hal::Port::C, 6U)
#define D2_MOTORA_CHB hal::Pin(hal::Port::C, 7U)
#define D2_MOTORB_CHA hal::Pin(hal::Port::A, 8U)
#define D2_MOTORB_CHB hal::Pin(hal::Port::A, 9U)

#define D2_AIN1 hal::Pin(hal::Port::B, 1u)
#define D2_AIN2 hal::Pin(hal::Port::B, 2u)

#define D2_BIN1 hal::Pin(hal::Port::C, 9u)
#define D2_BIN2 hal::Pin(hal::Port::C, 8u)

#define D2_STBY hal::Pin(hal::Port::A, 10u)

// clang-format off
//! @brief Debug UART
#define DEBUG_TX hal::GPIO(hal::Pin(hal::Port::B, 6U), hal::GpioMode::AF_PUSH_PULL, \
                 hal::GpioPullType::NO_PULL, GPIO_AF7_USART1, hal::GpioSpeed::VERY_HIGH)
#define DEBUG_RX hal::GPIO(hal::Pin(hal::Port::B, 7U), hal::GpioMode::AF_PUSH_PULL, \
                 hal::GpioPullType::NO_PULL, GPIO_AF7_USART1, hal::GpioSpeed::VERY_HIGH)

//! @brief IMU I2C
#define IMU_DEV_ADDR 0x69u
#define IMU_SCL hal::GPIO(hal::Pin(hal::Port::B, 8U), hal::GpioMode::AF_OPEN_DRAIN, \
                hal::GpioPullType::NO_PULL, GPIO_AF4_I2C1, hal::GpioSpeed::VERY_HIGH)
#define IMU_SDA hal::GPIO(hal::Pin(hal::Port::B, 9U), hal::GpioMode::AF_OPEN_DRAIN, \
                hal::GpioPullType::NO_PULL, GPIO_AF4_I2C1, hal::GpioSpeed::VERY_HIGH)
#define IMU_INT hal::Pin(hal::Port::C, 13u)

//! @brief NRF24 SPI
#define RF_MOSI hal::GPIO(hal::Pin(hal::Port::C, 12U), hal::GpioMode::AF_PUSH_PULL, \
                hal::GpioPullType::NO_PULL, GPIO_AF6_SPI3, hal::GpioSpeed::VERY_HIGH)
#define RF_MISO hal::GPIO(hal::Pin(hal::Port::C, 11U), hal::GpioMode::AF_PUSH_PULL, \
                hal::GpioPullType::NO_PULL, GPIO_AF6_SPI3, hal::GpioSpeed::VERY_HIGH)
#define RF_SCK  hal::GPIO(hal::Pin(hal::Port::C, 10U), hal::GpioMode::AF_PUSH_PULL, \
                hal::GpioPullType::NO_PULL, GPIO_AF6_SPI3, hal::GpioSpeed::VERY_HIGH)
#define RF_CS   hal::Pin(hal::Port::D, 2u)
#define RF_IRQ  hal::Pin(hal::Port::B, 4u)
#define RF_EN   hal::Pin(hal::Port::B, 5u)
// clang-format on

#define VBATT_ADC hal::AnalogIn(hal::Pin(hal::Port::A, 5u))
#define VBATT_R1 3.3f
#define VBATT_R2 1.0f
#define VBATT_SCALE_FACTOR ((3.3f / 4095) * ((VBATT_R1 + VBATT_R2) / (VBATT_R2)))

#define HSE_IN_PIN GPIO_PIN_0
#define HSE_IN_GPIO_PORT GPIOH

#define HSE_OUT_PIN GPIO_PIN_1
#define HSE_OUT_GPIO_PORT GPIOH

#define CAN2_RS_PIN GPIO_PIN_10
#define CAN2_RS_GPIO_PORT GPIOB

#endif  // IO_H
