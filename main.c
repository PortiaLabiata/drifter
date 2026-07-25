#include "time_measurement.h"
#include "driver_usart.h"
#include "hardware_config.h"
#include "gcs.h"
#include <stdint.h>

#define SYSID 1
#define COMPID 1

void heartbeat_hook(mavlink_message_t *msg) {
    mavlink_heartbeat_t pkt = {
        .custom_mode = 1,
        .type = 0,
        .autopilot = MAV_AUTOPILOT_GENERIC,
        .base_mode = MAV_MODE_MANUAL_ARMED,
        .system_status = 0,
    };    
    mavlink_msg_heartbeat_encode(SYSID, COMPID, msg, &pkt);
}

void manual_control_hook(mavlink_message_t *msg) {
    mavlink_manual_control_t decoded;
    mavlink_msg_manual_control_decode(msg, &decoded);
}

void rc_hook(mavlink_message_t *msg) {

}

int main() {
    configure_system();
    configure_usart();
    configure_pwm();

    gcs_set_out_hook(MAVLINK_MSG_ID_HEARTBEAT, heartbeat_hook);
    gcs_set_out_hook(MAVLINK_MSG_ID_RC_CHANNELS, rc_hook);

    gcs_set_in_hook(MAVLINK_MSG_ID_MANUAL_CONTROL, manual_control_hook);

    static uint8_t buffer[280];
    while (1) {
        ssize_t res = gcs_update_tx(buffer);
        if (res > 0) {
            usart_write(buffer, res);
        }

        uint8_t byte = 0x00;
        if (usart_read(&byte)) {
            gcs_update_rx(byte);
        }
        delay(20);
    }    
}

void HardFault_Handler() {
    while (1)
        ;
    
}    
