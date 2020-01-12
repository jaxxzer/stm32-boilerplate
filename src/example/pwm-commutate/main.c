#include <target-pwm.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

/*
 * This example outputs a six-step commutation signal for
 * brushless motors. This is performed with the Advanced Control
 * Timer and the commutation (COM) event.
 */

//  x = zero-cross  ------------------------------------------
//           | Step0 | Step1 | Step2 | Step3 | Step4 | Step5 |
//-----------------------------------------------------------
//|Channel1  |‾|_|‾|_|‾|_|‾|_________________________________|
//-------------------------------x-----------------------x---
//|Channel1N |_|‾|_|‾|_|‾|_|‾|_______|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|_______|
//-----------------------------------------------------------
//|Channel2  |_______________|‾|_|‾|_|‾|_|‾|_________________|
//-----------------------x-----------------------x------------
//|Channel2N |‾‾‾‾‾‾‾|_________|‾|_|‾|_|‾|_|‾|_______|‾‾‾‾‾‾‾|
//-----------------------------------------------------------
//|Channel3  |_______________________________|‾|_|‾|_|‾|_|‾|_|
//---------------x-----------------------x-------------------
//|Channel3N |_______|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|_________|‾|_|‾|_|‾|_|‾|

const uint16_t psc = 9;    // prescaler
const uint16_t arr = 1000; // auto-reload
const uint16_t ccr = 500;  // capture-compare (duty)

typedef struct {
  enum rcc_periph_clken rcc;
  uint32_t port;
  uint16_t pin;
  // STM32F1 series do not have alternate function mux
  // use AFIO if remap is necessary
#if !defined(STM32F1)
  uint8_t af;
#endif
} gpio_t;

#define NGPIO 6
#define ADVANCED_TIMER TIM1
#define ADVANCED_TIMER_RCC RCC_TIM1

gpio_t gpios[NGPIO] = {
#if defined(STM32F1)
    RCC_GPIOA, GPIOA, GPIO8,  RCC_GPIOA, GPIOA, GPIO9,  RCC_GPIOA, GPIOA, GPIO10,
    RCC_GPIOB, GPIOB, GPIO13, RCC_GPIOB, GPIOB, GPIO14, RCC_GPIOB, GPIOB, GPIO15,
#else
    BRIDGE_HI_A_GPIO_RCC,  BRIDGE_HI_A_GPIO_PORT, BRIDGE_HI_A_GPIO_PIN,  BRIDGE_HI_A_GPIO_AF,   BRIDGE_HI_B_GPIO_RCC,
    BRIDGE_HI_B_GPIO_PORT, BRIDGE_HI_B_GPIO_PIN,  BRIDGE_HI_B_GPIO_AF,   BRIDGE_HI_C_GPIO_RCC,  BRIDGE_HI_C_GPIO_PORT,
    BRIDGE_HI_C_GPIO_PIN,  BRIDGE_HI_C_GPIO_AF,   BRIDGE_LO_A_GPIO_RCC,  BRIDGE_LO_A_GPIO_PORT, BRIDGE_LO_A_GPIO_PIN,
    BRIDGE_LO_A_GPIO_AF,   BRIDGE_LO_B_GPIO_RCC,  BRIDGE_LO_B_GPIO_PORT, BRIDGE_LO_B_GPIO_PIN,  BRIDGE_LO_B_GPIO_AF,
    BRIDGE_LO_C_GPIO_RCC,  BRIDGE_LO_C_GPIO_PORT, BRIDGE_LO_C_GPIO_PIN,  BRIDGE_LO_C_GPIO_AF,
#endif
};

void setupGpio(gpio_t* gpio) {
#if defined(STM32F1)
  rcc_periph_clock_enable(RCC_AFIO); // don't forget to do this!
  rcc_periph_clock_enable(gpio->rcc);
  gpio_set_mode(gpio->port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, gpio->pin);
#else
  rcc_periph_clock_enable(gpio->rcc);
  gpio_mode_setup(gpio->port, GPIO_MODE_AF, GPIO_PUPD_NONE, gpio->pin);
  gpio_set_af(gpio->port, gpio->af, gpio->pin);
#endif
}

void setupTimer() {
  rcc_periph_clock_enable(ADVANCED_TIMER_RCC);
  timer_set_prescaler(ADVANCED_TIMER, psc);
  timer_set_period(ADVANCED_TIMER, arr);

  // load the compare values for the pwms (CCRx)
  timer_set_oc_value(ADVANCED_TIMER, TIM_OC1, ccr);
  timer_set_oc_value(ADVANCED_TIMER, TIM_OC2, ccr);
  timer_set_oc_value(ADVANCED_TIMER, TIM_OC3, ccr);

  // initialize OCxM
  timer_set_oc_mode(ADVANCED_TIMER, TIM_OC1, TIM_OCM_FORCE_LOW);
  timer_set_oc_mode(ADVANCED_TIMER, TIM_OC2, TIM_OCM_FORCE_LOW);
  timer_set_oc_mode(ADVANCED_TIMER, TIM_OC3, TIM_OCM_FORCE_LOW);

  // initialize OCxE and OCxNE
  timer_enable_oc_output(ADVANCED_TIMER, TIM_OC1);
  timer_enable_oc_output(ADVANCED_TIMER, TIM_OC2);
  timer_enable_oc_output(ADVANCED_TIMER, TIM_OC3);
  timer_enable_oc_output(ADVANCED_TIMER, TIM_OC1N);
  timer_enable_oc_output(ADVANCED_TIMER, TIM_OC2N);
  timer_enable_oc_output(ADVANCED_TIMER, TIM_OC3N);

  // set CCPC in CR2 register
  // when this bit is set, changes to OCxE, OCxNE, and OCxM are preloaded
  // and only take effect on a commutation (COM) event
  // this allows us to change all six signal outputs on the bridge
  // simultaneously as soon as the COMG bit is set in the event generation register (EGR)
  timer_enable_preload_complementry_enable_bits(ADVANCED_TIMER);

  // always set a dead time to avoid blowing up the bridge via shoot-through
  timer_set_deadtime(ADVANCED_TIMER, 0x40);

  // set these bits so that the timer maintains control of the outputs
  // on channels that are disabled
  timer_set_enabled_off_state_in_idle_mode(ADVANCED_TIMER); // get this bit wrong and blow your bridge
  timer_set_enabled_off_state_in_run_mode(ADVANCED_TIMER);

  // enable the pwm output to the bridge
  timer_enable_break_main_output(ADVANCED_TIMER);

  // start the timer counting
  timer_enable_counter(ADVANCED_TIMER);
}

int main() {
  initializeSystemClocks();

  for (uint8_t i = 0; i < NGPIO; i++) {
    setupGpio(gpios + i);
  }

  setupTimer();

  while (1) {
    uint8_t i;
    for (i = 0; i < 6; i++) {

      // small do-nothing delay
      for (uint32_t n = 0; n < 10000; n++) {
        asm("nop");
      }

      // control our six-step states via OCxE, OCxNE, and OCxM
      // these bits are preloaded because we set the CCPC bit in the
      // CR2 register. the output to the bridge will not change state
      // until we generate the commutation event by setting COMG in
      // the EGR register.
      switch (i) {
      case 0:
        timer_enable_oc_output(ADVANCED_TIMER, TIM_OC1);
        timer_disable_oc_output(ADVANCED_TIMER, TIM_OC3);
        timer_set_oc_mode(ADVANCED_TIMER, TIM_OC1, TIM_OCM_PWM1);
        timer_set_oc_mode(ADVANCED_TIMER, TIM_OC2, TIM_OCM_FORCE_LOW);
        timer_set_oc_mode(ADVANCED_TIMER, TIM_OC3, TIM_OCM_FORCE_LOW);
        break;
      case 1:
        timer_enable_oc_output(ADVANCED_TIMER, TIM_OC3);
        timer_disable_oc_output(ADVANCED_TIMER, TIM_OC2);
        break;
      case 2:
        timer_enable_oc_output(ADVANCED_TIMER, TIM_OC2);
        timer_disable_oc_output(ADVANCED_TIMER, TIM_OC1);
        timer_set_oc_mode(ADVANCED_TIMER, TIM_OC1, TIM_OCM_FORCE_LOW);
        timer_set_oc_mode(ADVANCED_TIMER, TIM_OC2, TIM_OCM_PWM1);
        timer_set_oc_mode(ADVANCED_TIMER, TIM_OC3, TIM_OCM_FORCE_LOW);
        break;
      case 3:
        timer_enable_oc_output(ADVANCED_TIMER, TIM_OC1);
        timer_disable_oc_output(ADVANCED_TIMER, TIM_OC3);
        break;
      case 4:
        timer_enable_oc_output(ADVANCED_TIMER, TIM_OC3);
        timer_disable_oc_output(ADVANCED_TIMER, TIM_OC2);
        timer_set_oc_mode(ADVANCED_TIMER, TIM_OC1, TIM_OCM_FORCE_LOW);
        timer_set_oc_mode(ADVANCED_TIMER, TIM_OC2, TIM_OCM_FORCE_LOW);
        timer_set_oc_mode(ADVANCED_TIMER, TIM_OC3, TIM_OCM_PWM1);
        break;
      case 5:
        timer_enable_oc_output(ADVANCED_TIMER, TIM_OC2);
        timer_disable_oc_output(ADVANCED_TIMER, TIM_OC1);
        break;
      }

      // generate a commutation event. all of the preloaded bits
      // above will take effect simultaneously to move to the next
      // commutation step in the six-step sequence.
      timer_generate_event(ADVANCED_TIMER, TIM_EGR_COMG);
    }
  }
}
