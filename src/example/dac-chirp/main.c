#include <target.h>

#if !defined(EXAMPLE_DAC)
# error "example not supported by platform"
#endif

#include <libopencm3/stm32/dac.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

uint16_t sine10bit(uint16_t index);

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

const uint16_t resolution = 256;
uint16_t sine10bit(uint16_t index)
{
  if (index > 0x800) {
    index = 0x800;
  }
  if (angle < resolution)
    return 2048 + lookup[angle];
  } else if (angle < resolution*2) {
    angle = angle - resolution;
    return 2048 + lookup[resolution - angle - 1];
  } else if (angle < 2*resolution) {
    angle = angle - 2*resolution;
    return 2048 - lookup[angle];
  } else if (angle < 3*resolution) {
    angle = angle - 3*resolution;
    return 2048 - lookup[resolution - angle - 1];
  }
}
