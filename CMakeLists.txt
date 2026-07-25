cmake_minimum_required(VERSION 3.13)
project(drifter LANGUAGES C ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/metal.cmake)
include(cmake/drivers.cmake)

add_executable(firmware.elf)
target_sources(firmware.elf PUBLIC
  main.c
  gcs.c
  time_measurement.c
  ring.c
  hardware_config.c)
target_link_libraries(firmware.elf PUBLIC
  metal drivers)
target_compile_options(firmware.elf PUBLIC
    -Wno-address-of-packed-member 
    -Og 
    -Wall 
    -Wextra 
    -Werror)

set(OCD_HOME /home/portia_labiata/ch32_toolchain/OpenOCD/OpenOCD)
set(TOOLCHAIN /home/portia_labiata/ch32_toolchain)

add_custom_target(
  size ALL
  DEPENDS firmware.elf bootloader.elf
  COMMAND ${TOOLCHAIN}/bin/riscv-wch-elf-size firmware.elf
  COMMAND ${TOOLCHAIN}/bin/riscv-wch-elf-size bootloader.elf)

add_custom_target(
  bin ALL
  DEPENDS firmware.elf bootloader.elf

  COMMAND ${TOOLCHAIN}/bin/riscv-wch-elf-objcopy -O binary
  firmware.elf firmware.bin

  COMMAND ${TOOLCHAIN}/bin/riscv-wch-elf-objcopy -O binary
  bootloader.elf bootloader.bin)

add_custom_target(
  copy_ini ALL
  COMMAND cp
  ${CMAKE_CURRENT_SOURCE_DIR}/metal/CH32V003F4P6.ini
  ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(
  flash
  COMMAND ${TOOLCHAIN}/minichlink
  -w firmware.bin flash)

add_custom_target(
  flash_bootloader
  COMMAND ${TOOLCHAIN}/minichlink
  -w bootloader.bin bootloader)

add_custom_target(
  debug
  COMMAND ${OCD_HOME}/bin/openocd
  -f ${OCD_HOME}/bin/wch-riscv.cfg)

add_custom_target(
    unbrick
    COMMAND ${TOOLCHAIN}/minichlink -u)
