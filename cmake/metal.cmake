add_library(metal STATIC)
add_executable(bootloader.elf)

set(TOOLCHAIN /home/portia_labiata/ch32_toolchain)
set(CMAKE_C_COMPILER ${TOOLCHAIN}/bin/riscv-wch-elf-gcc)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN}/bin/riscv-wch-elf-gcc)

target_sources(metal PRIVATE
  metal/core_riscv.c
  metal/startup.s
  metal/system_ch32v00x.c

  metal/drivers/ch32v00x_rcc.c)
target_include_directories(metal PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/metal
  ${CMAKE_CURRENT_SOURCE_DIR}/metal/drivers)
target_sources(bootloader.elf PRIVATE
  metal/bootloader.s)

set(COMPILE_OPTIONS
  -Os
  -flto
  -ffunction-sections
  -fdata-sections
  -fmessage-length=0
  -msmall-data-limit=8
  -fno-tree-loop-distribute-patterns

  -march=rv32ec
  -mabi=ilp32e
  -DCH32V00x=1

  -nostartfiles
  -nostdlib
  --specs=nano.specs
  --specs=nosys.specs)

set(LINK_OPTIONS
  ${COMPILE_OPTIONS}
  -Wl,--print-memory-usage
  -Wl,-Map=memory.map
  -Wl,-T${CMAKE_CURRENT_SOURCE_DIR}/metal/linker.ld)

target_compile_options(metal PUBLIC ${COMPILE_OPTIONS})
target_link_options(metal PUBLIC ${LINK_OPTIONS})

target_compile_options(bootloader.elf PUBLIC ${COMPILE_OPTIONS})
target_link_options(bootloader.elf PUBLIC ${LINK_OPTIONS})
