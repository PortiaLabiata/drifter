#ifndef DRIVER_TIM
#define DRIVER_TIM

#include "ch32v00x.h"
#include "common.h"
#include <stdint.h>

typedef struct {
    bool enabled;
    bool complementary;
    uint16_t ccr;
} tim_chan_config_s;

typedef struct {
    uint16_t period;
    uint16_t psc;
    tim_chan_config_s channels[4];
} tim_config_s;

void tim_begin(void);
void tim_config(tim_config_s *config);
void tim_chan_set_duty(uint8_t chan, uint16_t duty);
void tim_chan_set_disabled(uint8_t chan, bool state);

#endif
