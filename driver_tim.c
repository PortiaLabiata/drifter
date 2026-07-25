#include "driver_tim.h"
#include "configuration.h"
#include <stddef.h>

void tim_begin(void) {
#if TIM1_EN 
    RCC->APB2PCENR |= RCC_TIM1EN;
#endif
#if TIM2_EN
    RCC->APB1PCENR |= RCC_TIM2EN;
#endif
}

void tim_config(tim_config_s *config) {
    assert(config);

    TIM_TypeDef *tim = TIM1;
    tim->CTLR1 = 0; tim->CTLR2 = 0;

    tim->ATRLR = config->period;
    tim->PSC = config->psc;
    tim->SWEVGR |= TIM_UG;

    for (size_t i = 0; i < 4; i++) {
        tim_chan_config_s *chan = &config->channels[i];
        if (!chan->enabled) {
            continue;
        }
        
        if (chan->complementary) {
            tim->CCER |= (1 << (i*4 + 2));
        } else {
            tim->CCER |= (1 << i*4);
        }

        static const uint32_t pwm1_mode = 0b110;
        switch (i) {
            case 0:
                tim->CHCTLR1 |= TIM_OC1M_1 | TIM_OC1M_2;
                tim->CH1CVR = chan->ccr;
                break;
            case 1:
                tim->CHCTLR1 |= TIM_OC2M_1 | TIM_OC2M_2;
                tim->CH2CVR = chan->ccr;
                break;
            case 2:
                tim->CHCTLR2 |= TIM_OC3M_1 | TIM_OC3M_2;
                tim->CH3CVR = chan->ccr;
                break;
            case 4:
                tim->CHCTLR2 |= TIM_OC4M_1 | TIM_OC4M_2;
                tim->CH4CVR = chan->ccr;
                break;
            default:
                break;
        }
    }
    tim->BDTR |= TIM_MOE;
    tim->CTLR1 |= TIM_CEN;
}

void tim_chan_set_duty(uint8_t chan, uint16_t duty) {
    volatile uint32_t *reg = NULL;
    switch (chan) {
    case 0:
        reg = &TIM1->CH1CVR;
        break;
    case 1:
        reg = &TIM1->CH2CVR;
        break;
    case 2:
        reg = &TIM1->CH3CVR;
        break;
    case 3:
        reg = &TIM1->CH4CVR;
        break;
    default:
        return;
    }

    *reg = duty;
}
