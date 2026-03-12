add_library(drivers STATIC)
target_link_libraries(drivers PUBLIC metal)
target_sources(drivers PUBLIC
  driver_gpio.c)
