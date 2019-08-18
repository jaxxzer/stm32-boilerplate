include(${PROJECT_SOURCE_DIR}/src/target/f/f.cmake)

set(ISR_DIR ${TARGET_DIR}/isr/example-button-exti)

include_directories(${ISR_DIR})

set(ISR_SRC ${ISR_DIR}/isr.c)

set(EXAMPLE_SRC
  ${EXTI_SRC} ${GPIO_SRC} ${NVIC_SRC} ${RCC_SRC} ${ISR_SRC}
)
