#ifndef GCS_H
#define GCS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAVLINK_COMM_NUM_BUFFERS 1
#include "mavlink/common/mavlink.h"
#include "time_measurement.h"

typedef struct {
    time_period_t period;
    time_period_t last_sent_time;
    uint32_t id;
    void (*hook)(mavlink_message_t *msg);
} gcs_message_s;

extern gcs_message_s _out_messages[];
extern gcs_message_s _in_messages[];

typedef struct {
    mavlink_message_t current_msg;
    mavlink_status_t current_status;
} gcs_s;    

void gcs_update_rx(uint8_t c);
ssize_t gcs_update_tx(uint8_t *buffer);
void gcs_set_in_hook(uint8_t id, void (*hook)(mavlink_message_t *msg));
void gcs_set_out_hook(uint8_t id, void (*hook)(mavlink_message_t *msg));

#endif
