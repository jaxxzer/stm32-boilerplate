set(ADC_SRC
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/adc_common_v1.c
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/f1/adc.c
)

set(DMA_SRC
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/dma_common_l1f013.c
)

set(GPIO_SRC
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/gpio_common_all.c
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/f1/gpio.c
)

set(RCC_SRC
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/rcc_common_all.c
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/flash_common_all.c
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/f1/rcc.c
)

SET(TIMER_SRC
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/timer_common_all.c
)
