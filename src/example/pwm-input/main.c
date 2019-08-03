#include <target-pwm.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

/*
 * This example captures a pwm signal period and duty
 * The method employed here is described in the 'pwm input'
 * section of the stm32 reference manuals
 *
 * period is captured in CCR of PWM_TIMER_IC_ID_RISE
 * duty is captured in CCR of PWM_TIMER_IC_ID_FALL
 */

const uint16_t psc = 19;
const uint16_t arr = 0xffff;

void setupGpio() {
  rcc_periph_clock_enable(PWM_GPIO_RCC);

#if defined(STM32F1)
  rcc_periph_clock_enable(RCC_AFIO); // don't forget to do this!
  gpio_set_mode(PWM_GPIO_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, PWM_GPIO_PIN);
  gpio_primary_remap(AFIO_MAPR_SWJ_CFG_FULL_SWJ, PWM_GPIO_MAPR);
#else
  gpio_mode_setup(PWM_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, PWM_GPIO_PIN);
  gpio_set_af(PWM_GPIO_PORT, PWM_GPIO_AF, PWM_GPIO_PIN);
#endif
}

void setupTimer() {
  // trigger input 1 will:
  // 1. capture the signal period in CCR1 (rising edge - rising edge time)
  // 2. reset the counter (the timer keeps running)
  rcc_periph_clock_enable(PWM_TIMER_RCC); // enable timer clock
  // stretch clock with larger dividers in order to time longer signals without overruns
  timer_set_prescaler(PWM_TIMER, psc);
  timer_set_period(PWM_TIMER, arr);                                     // set ARR
  timer_ic_set_input(PWM_TIMER, PWM_TIMER_IC_ID_RISE, PWM_TIMER_IC_IN); // set both input channels to trigger input 1
  timer_ic_set_input(PWM_TIMER, PWM_TIMER_IC_ID_FALL, PWM_TIMER_IC_IN);
  // set second input channel trigger polarity to falling (rising is default, first input channel is rising)
  // input/output configurations are on the same register (so we use set oc_polarity..)
  timer_set_oc_polarity_low(PWM_TIMER, PWM_TIMER_OC_ID_FALL);

  timer_ic_enable(PWM_TIMER, PWM_TIMER_IC_ID_RISE);
  timer_ic_enable(PWM_TIMER, PWM_TIMER_IC_ID_FALL);

  // set slave mode, reset counter on trigger
  timer_slave_set_mode(PWM_TIMER, PWM_TIMER_SLAVE_MODE);

  // set slave mode trigger to trigger input 1
  timer_slave_set_trigger(PWM_TIMER, PWM_TIMER_SLAVE_TS);

  timer_enable_counter(PWM_TIMER); // set CEN
}

int main() {
  initializeSystemClocks();
  setupGpio();
  setupTimer();
  while (1) {
  };
}
