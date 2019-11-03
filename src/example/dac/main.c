#include <target.h>

#if !defined(EXAMPLE_DAC)
# error "example not supported by platform"
#endif

#include <libopencm3/stm32/dac.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

int main() {
  initializeSystemClocks();

  // configure gpio pin
  rcc_periph_clock_enable(DAC_GPIO_RCC);
  gpio_mode_setup(DAC_GPIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, DAC_GPIO_PIN);

  // configure DAC
  rcc_periph_clock_enable(DAC_RCC);
  dac_enable(CHANNEL_1);

  int16_t dacOut = 0;
  int16_t increment = 1;

  while (1) {
    dacOut += increment;
    if (dacOut >= 0xfff) {
      dacOut = 0xfff;
      increment = -2; // fall @ 2x rate of rise
    } else if (dacOut <= 0) {
      dacOut = 0;
      increment = 1;
    }
    dac_load_data_buffer_single(dacOut, LEFT12, CHANNEL_1);
  }
}
