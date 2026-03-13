#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdbool.h>

#ifndef RELEASE
#define assert(v)                                                       \
    do {                                                                \
      if (!(v)) {                                                       \
          while (1)                                                     \
              ;                                                         \
      }                                                                 \
  } while (0)

#else
#define assert(v)
#endif

typedef uint8_t bitflag_t;
typedef uint32_t register_value_t;
typedef uint32_t time_period_t;

#define PACKED __attribute__((packed))
#define ALIGNED __attribute__((aligned))
#define BITFIELD(name) bitflag_t name : 1;

#endif
