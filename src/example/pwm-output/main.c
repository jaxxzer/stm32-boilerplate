#include <target-pwm.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

/*
 * This example outputs a static pwm signal on a gpio
 *
 * The gpio is configured as alternate function + output
 *
 * When psc = 0, arr = 1, ccr = 1, the pwm signal is 50% duty,
 * and the frequency is one-half of the timer counter frequency
 */

const uint16_t psc = 19; // prescaler
const uint16_t arr = 1;  // auto-reload
const uint16_t ccr = 1;  // compare

void setupGpio() {
  rcc_periph_clock_enable(PWM_GPIO_RCC);

#if defined(STM32F1)
  rcc_periph_clock_enable(RCC_AFIO); // don't forget to do this!
  gpio_set_mode(PWM_GPIO_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, PWM_GPIO_PIN);
  // gpios a13 and a14 are reserved for SWD, and may not be used
  gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON, PWM_GPIO_MAPR);
#else
  gpio_mode_setup(PWM_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, PWM_GPIO_PIN);
  gpio_set_af(PWM_GPIO_PORT, PWM_GPIO_AF, PWM_GPIO_PIN);
  gpio_set_output_options(PWM_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PWM_GPIO_PIN);
#endif
}

void setupTimer() {
  rcc_periph_clock_enable(PWM_TIMER_RCC);
  // stretch clock with larger dividers in order to time longer signals without overruns
  timer_set_prescaler(PWM_TIMER, psc);
  timer_set_period(PWM_TIMER, arr);
  timer_set_oc_value(PWM_TIMER, PWM_TIMER_OC_ID, ccr);
  timer_set_oc_mode(PWM_TIMER, PWM_TIMER_OC_ID, TIM_OCM_PWM1);
  timer_enable_oc_preload(PWM_TIMER, PWM_TIMER_OC_ID);
  timer_enable_oc_output(PWM_TIMER, PWM_TIMER_OC_ID);
  timer_enable_break_main_output(PWM_TIMER); // shouldn't do/be part of f0 aapi?

  timer_enable_counter(PWM_TIMER);
}

int main() {
  initializeSystemClocks();
  setupGpio();
  setupTimer();
  while (1) {
  };
}
