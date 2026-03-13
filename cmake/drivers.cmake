add_library(drivers STATIC)
target_link_libraries(drivers PRIVATE metal)
target_sources(drivers PRIVATE
  driver_gpio.c
  driver_usart.c)
