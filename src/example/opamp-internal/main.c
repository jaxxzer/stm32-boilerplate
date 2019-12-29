#include <target.h>

#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include <libopencm3/stm32/g4/opamp.h>

uint8_t channel_array[] = {ADC_OPAMP_CHANNEL};

void led_setup();
void adc_setup();

int main() {
  initializeSystemClocks();
  led_setup();

  rcc_periph_clock_enable(RCC_SYSCFG);

  // select pga mode on inverting input
  OPAMP_CSR(OPAMP1) |= OPAMP_CSR_VM_SEL_PGA;
  // non inverting gain = 2
  OPAMP1_CSR |= (0b00000 << OPAMP_CSR_PGA_GAIN_SHIFT);
  // connect opamp vp to VINP0 signal
  OPAMP1_CSR |= (0b00 << OPAMP_CSR_VP_SEL_SHIFT);
  // connect the opamp internally
  OPAMP1_CSR |= OPAMP_CSR_OPAINTOEN;
  // enable high speed mode
  OPAMP1_CSR |= OPAMP_CSR_OPAHSM;
  // enable the opamp
  OPAMP1_CSR |= 1;

  adc_setup();
  adc_start_conversion_regular(ADC_PERIPH);

  uint32_t count;
  uint16_t adcValue;

  while (1) {
    adcValue = adc_read_regular(ADC_PERIPH);

    if (count++ > adcValue * 50) {
      gpio_toggle(LED_GPIO_PORT, LED_GPIO_PIN);
      count = 0;
    }
  }
}

void adc_setup() {
  rcc_periph_clock_enable(ADC_GPIO_RCC);

#if defined(STM32F1)
  gpio_set_mode(ADC_GPIO_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG, ADC_GPIO_PIN);
#else
  gpio_mode_setup(ADC_GPIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_GPIO_PIN);
#endif

  rcc_periph_clock_enable(ADC_RCC);

#if defined(STM32F3) || defined(STM32G0) || defined(STM32G4)
  adc_enable_regulator(ADC_PERIPH);
  for (long i = 0; i < 10000; i++)
    asm("nop");
  adc_set_clk_prescale(ADC_PERIPH, ADC_CCR_PRESCALE);
#endif
  adc_set_continuous_conversion_mode(ADC_PERIPH);

#if defined(STM32F1)
  adc_power_on(ADC_PERIPH);
  adc_enable_external_trigger_regular(ADC_PERIPH, ADC_CR2_EXTSEL_SWSTART);
  adc_enable_scan_mode(ADC_PERIPH);
  adc_calibrate(ADC_PERIPH);
#else
  adc_calibrate(ADC_PERIPH);
  adc_power_on(ADC_PERIPH);
#endif

  adc_set_regular_sequence(ADC_PERIPH, 1, channel_array);
  adc_set_continuous_conversion_mode(ADC_PERIPH);
  ADC_CFGR1(ADC_PERIPH) |= ADC_CFGR1_OVRMOD;

  for (long i = 0; i < 10000; i++)
    asm("nop");
}

void led_setup() {
  rcc_periph_clock_enable(LED_GPIO_RCC);

#if defined(STM32F1)
  gpio_set_mode(LED_GPIO_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED_GPIO_PIN);
#else
  gpio_mode_setup(LED_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_GPIO_PIN);
#endif

  gpio_set(LED_GPIO_PORT, LED_GPIO_PIN);
}
