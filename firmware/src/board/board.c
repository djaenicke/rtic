#include "board.h"

#include <stdint.h>

#include "cmsis_os.h"

static void configSystemClock(void);
static void errorHandler(void);

void initBoard(void)
{
  HAL_Init();
  configSystemClock();
}

void delayMs(const uint32_t delay_ms)
{
  const TickType_t start_ticks = xTaskGetTickCount();
  const TickType_t delay_ticks = delay_ms * portTICK_PERIOD_MS;
  while ((xTaskGetTickCount() - start_ticks) < delay_ticks)
  {
  }
}

static void configSystemClock(void)
{
  RCC_OscInitTypeDef rcc_osc_init = { 0 };
  RCC_ClkInitTypeDef rcc_clk_init = { 0 };

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  rcc_osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  rcc_osc_init.HSEState = RCC_HSE_ON;
  rcc_osc_init.PLL.PLLState = RCC_PLL_ON;
  rcc_osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  rcc_osc_init.PLL.PLLM = 8;
  rcc_osc_init.PLL.PLLN = 72;
  rcc_osc_init.PLL.PLLP = RCC_PLLP_DIV2;
  rcc_osc_init.PLL.PLLQ = 3;
  rcc_osc_init.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&rcc_osc_init) != HAL_OK)
  {
    errorHandler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  rcc_clk_init.ClockType =
      RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  rcc_clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  rcc_clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
  rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV1;
  rcc_clk_init.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_LATENCY_0) != HAL_OK)
  {
    errorHandler();
  }
}

static void errorHandler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
