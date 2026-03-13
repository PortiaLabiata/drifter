#include "time_measurement.h"
#include "driver_usart.h"
#include "driver_gpio.h"
#include "gcs.h"

void heartbeat_hook(mavlink_message_t *msg) {
    mavlink_heartbeat_t pkt = {
        .custom_mode = 0,
        .type = 0,
        .autopilot = MAV_AUTOPILOT_INVALID,
        .base_mode = 0,
        .system_status = 0,
    };    
    mavlink_msg_heartbeat_encode(123, 123, msg, &pkt);
}

int main() {
    time_begin();
    gpio_begin();
    gpio_configure(GPIOD, 5,
                   GPIO_MODE_OUTPUT_10MHZ, GPIO_CNF_AF_PP);
    gpio_configure(GPIOD, 6,
                   GPIO_MODE_INPUT, GPIO_CNF_FLOATING);

    usart_begin();
    
    usart_config_s usart_cfg = {
        .speed = 115200,
        .word_length = USART_WORD_LENGTH_8BIT,
        .parity = USART_PARITY_DISABLED,
    };
    usart_configure(&usart_cfg);
    
    _out_messages[0].hook = heartbeat_hook;

    static uint8_t buffer[280];
    while (1) {
        ssize_t res = gcs_update_tx(buffer);
        if (res > 0)
            usart_write_blocking(buffer, res);
        delay(20);
    }    
}

void HardFault_Handler() {
    while (1)
        ;
    
}    
