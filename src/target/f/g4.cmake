set(GPIO_SRC
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/gpio_common_all.c
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/gpio_common_f0234.c
)

set(RCC_SRC
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/rcc_common_all.c
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/flash_common_all.c
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/g4/rcc.c
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/g4/pwr.c
)
