#include <target-pwm.h>

#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/g0/dmamux.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#include <inttypes.h>

/*
 * This example outputs a dynamic pwm signal on a gpio
 *
 * The pwm pulse width changes after every compare-match event
 * when the compare matches (pulse falls), the next pulse width is
 * loaded into the compare register (ccr) via dma
 *
 * dma reads the pulse widths sequentially from a buffer
 *
 * This method can be used to efficiently implement one wire signals
 * like ws2812b leds, dshot ESCs, and even pulse-count modulated
 * (PCM) wav audio
 */

const uint16_t psc = 19; // prescaler

// A bit-frame is a constant time window of duration t
// a 1 bit is indicated by a pulse of duration 2t/3
// a 0 bit is indicated by a pulse of duration t/3

// pulse widths, in timer counts
// minimum is 0x8 when dma clock speed == timer clock speed (oddly, 0x3 works correctly too!)
const uint16_t bit_0      = 0;
const uint16_t bit_1      = 101;
const uint16_t bit_period = 100; // symbol duration

const uint16_t pulses[] = {bit_0, bit_1, bit_0, bit_1, bit_0, bit_0, bit_1, bit_1};

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
  rcc_periph_clock_enable(PWM_TIMER_RCC); // enable timer clock
  // stretch clock with larger dividers in order to time longer signals without overruns
  timer_set_prescaler(PWM_TIMER, psc);
  timer_set_period(PWM_TIMER, bit_period);                     // set ARR
  timer_set_oc_mode(PWM_TIMER, PWM_TIMER_OC_ID, TIM_OCM_PWM1); // set OCM
  // timer_enable_oc_preload(PWM_TIMER, PWM_TIMER_OC_ID);         // set OCPE
  timer_enable_oc_output(PWM_TIMER, PWM_TIMER_OC_ID); // set CCxE
  // shouldn't do/be part of normal timer api (this only applies to advanced control timers)?
  timer_enable_break_main_output(PWM_TIMER);
  timer_enable_irq(PWM_TIMER, PWM_DMA_REQUEST); // set CCxDE (enable dma request on CCx event)
  timer_enable_counter(PWM_TIMER);              // set CEN
}

void setupDma() {
  rcc_periph_clock_enable(PWM_DMA_RCC);                                            // enable dma clock
  dma_set_peripheral_address(PWM_DMA, PWM_DMA_CHANNEL, (uint32_t)&PWM_DMA_PERIPH); // set CPAR
  dma_set_memory_address(PWM_DMA, PWM_DMA_CHANNEL, (uint32_t)&pulses);             // set CMAR
  dma_set_read_from_memory(PWM_DMA, PWM_DMA_CHANNEL);                              // set DIR
  dma_set_memory_size(PWM_DMA, PWM_DMA_CHANNEL, DMA_CCR_MSIZE_16BIT);              // set MSIZE
  dma_set_peripheral_size(PWM_DMA, PWM_DMA_CHANNEL, PWM_DMA_PERIPH_SIZE);          // set PSIZE
  dma_enable_memory_increment_mode(PWM_DMA, PWM_DMA_CHANNEL);                      // set MINC
  dma_set_number_of_data(PWM_DMA, PWM_DMA_CHANNEL, sizeof(pulses) / 2);            // set CNDTR
  dma_enable_circular_mode(PWM_DMA, PWM_DMA_CHANNEL);                              // set CIRC

#if defined(STM32G0)
  DMAMUX_CxCR(DMAMUX1, PWM_DMA_CHANNEL) |= PWM_DMAMUX_REQID;
#endif

  dma_enable_channel(PWM_DMA, PWM_DMA_CHANNEL); // set EN
}

int main() {
  initializeSystemClocks();
  setupGpio();
  setupDma();
  setupTimer();
  while (1) {
  };
}
