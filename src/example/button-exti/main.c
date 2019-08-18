#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>

#include <target.h>

#include <isr.h>

void button_isr() {
  // clear flag
  exti_reset_request(BUTTON_EXTI);

  gpio_toggle(LED_GPIO_PORT, LED_GPIO_PIN);
}

int main() {
  // setup led gpio
  rcc_periph_clock_enable(LED_GPIO_RCC);

#if defined(STM32F1)
  gpio_set_mode(LED_GPIO_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED_GPIO_PIN);
#else
  gpio_mode_setup(LED_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_GPIO_PIN);
#endif

  // setup button gpio
  rcc_periph_clock_enable(BUTTON_GPIO_RCC);

#if defined(STM32F1)
  gpio_set_mode(BUTTON_GPIO_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, BUTTON_GPIO_PIN);
#else
  gpio_mode_setup(BUTTON_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, BUTTON_GPIO_PIN);
#endif

  // enable exti irq
  nvic_enable_irq(BUTTON_EXTI_IRQ);

  // setup exti
  exti_select_source(BUTTON_EXTI, BUTTON_GPIO_PORT);
  exti_set_trigger(BUTTON_EXTI, EXTI_TRIGGER_FALLING);
  exti_enable_request(BUTTON_EXTI);

  // done!
  while (1) {
  };
}
