#include <target-pwm.h>

#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#if defined(STM32G0)
#include <libopencm3/stm32/g0/dmamux.h>
#endif

#include <inttypes.h>

/*
 * This example outputs a one wire signal to digital addressable rgb
 * leds like the ws2812b (adafruit neopixel) or sk6812
 *
 * A bit-frame is a constant time window of duration 1250 nanoseconds
 * a 1 bit is indicated by a pulse of duration 825 nanoseconds
 * a 0 bit is indicated by a pulse of duration 425 nanoseconds
 *
 * The pwm pulse width changes after every compare-match event
 * when the compare matches (pulse falls), the next pulse width is
 * loaded into the compare register (ccr) via dma
 *
 * dma reads the pulse widths sequentially from a buffer
 */

// pulse widths, in ahb clock counts
float nsPerTick() { return 1000000000.0 / rcc_ahb_frequency; }

// nanoseconds
const uint16_t bit_0      = 425;
const uint16_t bit_1      = 825;
const uint16_t bit_period = 1250; // symbol duration

// 24 for 8-bit rgb and 1 byte for a trailing 0 (to pull the wire low between frames)
uint16_t pulses[25];

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
  rcc_periph_clock_enable(PWM_TIMER_RCC);                      // enable timer clock
  timer_set_period(PWM_TIMER, bit_period / nsPerTick());       // set ARR
  timer_set_oc_mode(PWM_TIMER, PWM_TIMER_OC_ID, TIM_OCM_PWM1); // set OCM
  timer_enable_oc_preload(PWM_TIMER, PWM_TIMER_OC_ID);         // set OCPE
  timer_enable_oc_output(PWM_TIMER, PWM_TIMER_OC_ID);          // set CCxE
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

  // #if defined(STM32G4)
  //   enable rcc peripheral clock for dmamux
  //   *(uint32_t*)0x40021048 |= 0b100;
  // #endif

#if defined(STM32G0) || defined(STM32G4)
  DMAMUX_CxCR(DMAMUX1, PWM_DMA_CHANNEL) = PWM_DMAMUX_REQID;
#endif

  dma_enable_channel(PWM_DMA, PWM_DMA_CHANNEL); // set EN
}

// update memory output buffer for dma transfer
void writeRGB(uint8_t r, uint8_t g, uint8_t b);

int main() {
  initializeSystemClocks();
  setupGpio();
  setupDma();
  setupTimer();
  uint8_t r, g, b;
  while (1) {
    while (!dma_get_interrupt_flag(PWM_DMA, PWM_DMA_CHANNEL, DMA_TCIF)) {
    }
    long interval = 9000000 / nsPerTick();
    for (long i = 0; i < interval; i++) {
      asm("nop");
    }
    writeRGB(r++, g, b);
    g += 7;
    b += 3;

    dma_disable_channel(PWM_DMA, PWM_DMA_CHANNEL);
    dma_clear_interrupt_flags(PWM_DMA, PWM_DMA_CHANNEL, DMA_TCIF);
    dma_set_number_of_data(PWM_DMA, PWM_DMA_CHANNEL, sizeof(pulses) / 2); // set CNDTR
    dma_enable_channel(PWM_DMA, PWM_DMA_CHANNEL);
  }
}

void writeRGB(uint8_t r, uint8_t g, uint8_t b) {
  uint16_t b0 = bit_0 / nsPerTick();
  uint16_t b1 = bit_1 / nsPerTick();

  for (uint8_t i = 0; i < 8; i++) {
    pulses[i] = g & (1 << (7 - i)) ? b1 : b0;
  }
  for (uint8_t i = 0; i < 8; i++) {
    pulses[8 + i] = r & (1 << (7 - i)) ? b1 : b0;
  }
  for (uint8_t i = 0; i < 8; i++) {
    pulses[16 + i] = b & (1 << (7 - i)) ? b1 : b0;
  }
}
