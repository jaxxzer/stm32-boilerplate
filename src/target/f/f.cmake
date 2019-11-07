if(NOT BOILERPLATE)
  set(BOILERPLATE ${PROJECT_SOURCE_DIR})
endif()
# check architecture, f0 etc..
if(TARGET_MCU MATCHES "STM32F0.*")
  include(${BOILERPLATE}/src/target/f/f0.cmake)
elseif(TARGET_MCU MATCHES "STM32G0.*")
  include(${BOILERPLATE}/src/target/f/g0.cmake)
elseif(TARGET_MCU MATCHES "STM32F1.*")
  include(${BOILERPLATE}/src/target/f/f1.cmake)
elseif(TARGET_MCU MATCHES "STM32F3.*")
  include(${BOILERPLATE}/src/target/f/f3.cmake)
elseif(TARGET_MCU MATCHES "STM32L4.*")
  include(${BOILERPLATE}/src/target/f/l4.cmake)
else()
  message("!!! ERROR no support for TARGET_MCU ${TARGET_MCU} !!!")
endif()

set(NVIC_SRC
  ${BOILERPLATE}/lib/libopencm3/lib/cm3/nvic.c
)

set(EXTI_SRC
  ${BOILERPLATE}/lib/libopencm3/lib/stm32/common/exti_common_all.c
)
