#include <target.h>

#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

uint8_t channel_array[] = {ADC_CHANNEL};

void led_setup();
void adc_setup();

int main() {
  initializeSystemClocks();
  led_setup();
  adc_setup();

  adc_set_watchdog_high_threshold(ADC1, 0xf80);
  adc_set_watchdog_low_threshold(ADC1, 0xff);
  adc_enable_analog_watchdog_on_selected_channel(ADC1, ADC_CHANNEL);

  uint32_t count;
  uint16_t adcValue;

  while (1) {
    adc_start_conversion_regular(ADC_PERIPH);

    while (!adc_eoc(ADC_PERIPH)) {
    };

    adcValue = adc_read_regular(ADC_PERIPH);

#if defined(STM32F1)
    if (ADC_SR(ADC1) & ADC_SR_AWD) {
      gpio_clear(LED_GPIO_PORT, LED_GPIO_PIN);
      ADC_SR(ADC1) = ADC_SR_AWD;
#else
    if (ADC_ISR(ADC1) & ADC_ISR_AWD1) {
      gpio_clear(LED_GPIO_PORT, LED_GPIO_PIN);
      ADC_ISR(ADC1) = ADC_ISR_AWD1;
#endif
    } else {
      if (count++ > adcValue * 10) {
        gpio_toggle(LED_GPIO_PORT, LED_GPIO_PIN);
        count = 0;
      }
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

#if defined(STM32F3)
  adc_enable_regulator(ADC_PERIPH);
  for (long i = 0; i < 10000; i++)
    asm("nop");
  adc_set_clk_prescale(ADC_PERIPH, ADC_CCR_CKMODE_DIV1);
#endif

  adc_set_regular_sequence(ADC_PERIPH, 1, channel_array);
  adc_calibrate(ADC_PERIPH);
  adc_power_on(ADC_PERIPH);
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
