cmake_minimum_required(VERSION 3.12)
project(stm32-boilerplate)

set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")

set(COMPILER_FLAGS "-fdata-sections -ffunction-sections -O0 -DNDEBUG -Werror")
set(LINKER_FLAGS "-nostartfiles -specs=nano.specs -specs=nosys.specs --static -ggdb3 -Wl,--gc-sections -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group -Wl,-Map=output.map -T ${PROJECT_SOURCE_DIR}/src/link/stm32-mem.ld -L ${PROJECT_SOURCE_DIR}/lib/libopencm3/lib")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# target board
if(NOT DEFINED TARGET_BOARD)
  set(TARGET_BOARD "nucleo-f042")
  message("no TARGET_BOARD defined, using ${TARGET_BOARD}")
endif()

# example program
if(NOT DEFINED EXAMPLE)
  set(EXAMPLE "blink")
  message("no EXAMPLE defined, using ${EXAMPLE}")
endif()

# target board directory
set(TARGET_DIR "${PROJECT_SOURCE_DIR}/src/target/${TARGET_BOARD}")

# include target board CMakeLists
include(${TARGET_DIR}/CMakeLists.txt)

# include target board defintions
include_directories(${TARGET_DIR})

# target part
# TODO fail if not set
message("TARGET_MCU: ${TARGET_MCU}")

include(${PROJECT_SOURCE_DIR}/src/target-mcu.cmake)

# target architecture
# TODO fail if not set
message("TARGET_CPU: ${TARGET_CPU}")

include_directories(${PROJECT_SOURCE_DIR}/lib/libopencm3/include)

# include example sources
include(${PROJECT_SOURCE_DIR}/src/example/${EXAMPLE}/CMakeLists.txt)

set(CMAKE_C_FLAGS "${COMPILER_FLAGS} ${ARCH_FLAGS}")
set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS} ${ARCH_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS} ${ARCH_FLAGS}")

set(COMMON_SRC ${PROJECT_SOURCE_DIR}/lib/libopencm3/lib/cm3/vector.c)

set(EXE_NAME ${TARGET_BOARD}-example-${EXAMPLE})

add_executable(${EXE_NAME} ${PROJECT_SOURCE_DIR}/src/example/${EXAMPLE}/main.c ${EXAMPLE_SRC} ${COMMON_SRC})

add_custom_command(TARGET ${EXE_NAME} PRE_LINK
  COMMAND python ${PROJECT_SOURCE_DIR}/src/link/generate-ldscript.py -p ${TARGET_MCU} > ${PROJECT_SOURCE_DIR}/src/link/stm32-mem.ld
)

add_custom_command(TARGET ${EXE_NAME} POST_BUILD
  COMMAND arm-none-eabi-size ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXE_NAME}
  COMMAND cp ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXE_NAME} debug.elf
)

# 'make flash' to upload to board after compiling
add_custom_target(flash
  # try stlink v2 first, then stlink v2-1 if that fails
  openocd -f interface/stlink-v2.cfg -f ${OPENOCD_TARGET} -c "program debug.elf verify reset exit" ||
  openocd -f interface/stlink-v2-1.cfg -f ${OPENOCD_TARGET} -c "program debug.elf verify reset exit"
  DEPENDS ${EXE_NAME}
)