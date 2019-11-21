set(ADC_SRC
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/adc_common_v2.c
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/adc_common_v2_multi.c
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/l4/adc.c
)

set(DMA_SRC
${BOILERPLATE}/lib/libopencm3/lib/stm32/common/dma_common_l1f013.c
${BOILERPLATE}/lib/libopencm3/lib/stm32/common/dma_common_csel.c
)

set(GPIO_SRC
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/gpio_common_all.c
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/gpio_common_f0234.c
)

set(RCC_SRC
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/rcc_common_all.c
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/flash_common_all.c
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/flash_common_idcache.c
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/l4/flash.c
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/l4/rcc.c
)

SET(TIMER_SRC
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/timer_common_all.c
)

SET(USART_SRC
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/usart_common_all.c
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/usart_common_v2.c
)
