/*
 * this example uses the internal opamp1 in PGA mode, where
 * the signal present on VINP0 (PA1) is amplified with gain=2
 * the output signal is connected internally to the adc, and is
 * disconnected to the gpio for the opamp output, so that gpio
 * can be used for other puroposes. VINP0 (PA1) must be configured
 * in analog mode (default configuration at reset, so we don't tend
 * to that here)
 */

#include <target.h>

#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include <libopencm3/stm32/g4/opamp.h>

uint8_t channel_array[] = {ADC_OPAMP_CHANNEL};

void led_setup();
void adc_setup();
void opamp_setup();

int main() {
  initializeSystemClocks();
  led_setup();
  opamp_setup();
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
  gpio_mode_setup(ADC_GPIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_GPIO_PIN);

  rcc_periph_clock_enable(ADC_RCC);

  adc_enable_regulator(ADC_PERIPH);
  for (long i = 0; i < 10000; i++)
    asm("nop");

  adc_set_clk_prescale(ADC_PERIPH, ADC_CCR_PRESCALE);
  adc_set_continuous_conversion_mode(ADC_PERIPH);
  adc_calibrate(ADC_PERIPH);
  adc_power_on(ADC_PERIPH);
  adc_set_regular_sequence(ADC_PERIPH, 1, channel_array);
  adc_set_continuous_conversion_mode(ADC_PERIPH);
  ADC_CFGR1(ADC_PERIPH) |= ADC_CFGR1_OVRMOD;

  for (long i = 0; i < 10000; i++)
    asm("nop");
}

void led_setup() {
  rcc_periph_clock_enable(LED_GPIO_RCC);
  gpio_mode_setup(LED_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_GPIO_PIN);
  gpio_set(LED_GPIO_PORT, LED_GPIO_PIN);
}

void opamp_setup() {
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
}
