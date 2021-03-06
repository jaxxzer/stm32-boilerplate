if(TARGET_MCU MATCHES "STM32.*")
  message("STM32 target")
  if(TARGET_MCU MATCHES "STM32F0.*")
    set(TARGET_CPU "cortex-m0")
    set(ARCH_FLAGS "-mcpu=cortex-m0 -mthumb -mfpu=vfp")
    set(OPENOCD_TARGET target/stm32f0x.cfg)
    set(IRQ_JSON ./include/libopencm3/stm32/f0/irq.json)
    # select libopencm3 target
    add_compile_definitions(STM32F0)
  elseif(${TARGET_MCU} MATCHES "STM32G0.*")
    set(TARGET_CPU "cortex-m0plus")
    set(ARCH_FLAGS "-mcpu=cortex-m0plus -mthumb -mfpu=vfp")
    set(OPENOCD_TARGET target/stm32g0x.cfg)
    set(IRQ_JSON ./include/libopencm3/stm32/g0/irq.json)
    # select libopencm3 target
    add_definitions(-DSTM32G0)
  elseif(TARGET_MCU MATCHES "STM32F1.*")
    set(TARGET_CPU "cortex-m3")
    set(ARCH_FLAGS "-mcpu=cortex-m3 -mthumb -mfpu=vfp")
    set(IRQ_JSON ./include/libopencm3/stm32/f1/irq.json)
    set(OPENOCD_TARGET target/stm32f1x.cfg)
    # select libopencm3 target
    add_compile_definitions(STM32F1)
  elseif(TARGET_MCU MATCHES "STM32F3.*")
    set(TARGET_CPU "cortex-m4")
    set(ARCH_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=vfp")
    set(IRQ_JSON ./include/libopencm3/stm32/f3/irq.json)
    set(OPENOCD_TARGET target/stm32f3x.cfg)
    # select libopencm3 target
    add_compile_definitions(STM32F3)
  elseif(TARGET_MCU MATCHES "STM32L4.*")
    set(TARGET_CPU "cortex-m4")
    set(ARCH_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=vfp")
    set(IRQ_JSON ./include/libopencm3/stm32/l4/irq.json)
    set(OPENOCD_TARGET target/stm32l4x.cfg)
    # select libopencm3 target
    add_compile_definitions(STM32L4)
  elseif(TARGET_MCU MATCHES "STM32G4.*")
    set(TARGET_CPU "cortex-m4")
    set(ARCH_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=vfp")
    set(IRQ_JSON ./include/libopencm3/stm32/g4/irq.json)
    set(OPENOCD_TARGET target/stm32g4x.cfg)
    # select libopencm3 target
    add_compile_definitions(STM32G4)
  else()
    error("NO SUPPORT for STM32 target mcu: ${TARGET_MCU}")
  endif()
endif()
