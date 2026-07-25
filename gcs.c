#include "gcs.h"
#include "mavlink/common/mavlink_msg_rc_channels_override.h"

static gcs_s _gcs;

#define _out_messages_num (size_t)2
#define _in_messages_num (size_t)3

static gcs_message_s _out_messages[_out_messages_num] = {
    {
        .period = 100, 
        .last_sent_time = 0, 
        .id = MAVLINK_MSG_ID_RC_CHANNELS, 
        .hook = NULL
    },
    {
        .period = 1000, 
        .last_sent_time = 0, 
        .id = MAVLINK_MSG_ID_HEARTBEAT, 
        .hook = NULL
    },
};

static gcs_message_s _in_messages[_in_messages_num] = {
    {
        .period = 0, 
        .last_sent_time = 0, 
        .id = MAVLINK_MSG_ID_MANUAL_CONTROL, 
        .hook = NULL
    },
    {
        .period = 0,
        .last_sent_time = 0,
        .id = MAVLINK_MSG_ID_RC_CHANNELS_OVERRIDE,
        .hook = NULL
    },
    {
        .period = 0, 
        .last_sent_time = 0, 
        .id = MAVLINK_MSG_ID_HEARTBEAT, 
        .hook = NULL
    },
};

static bool _process_message(mavlink_message_t *msg) {
    for (size_t i = 0; i < _in_messages_num; i++) {
        gcs_message_s *_msg_s = &_in_messages[i];
        if (msg->msgid == _msg_s->id) {
            if (_msg_s->hook) {
                _msg_s->hook(msg);
            }
            return true;
        }        
    }    
    return false;
}  

void gcs_update_rx(uint8_t c) {
    uint8_t res = mavlink_parse_char(0, c, &_gcs.current_msg,
                                     &_gcs.current_status);
    if (res) {
        (void)_process_message(&_gcs.current_msg);
    }
}

ssize_t gcs_update_tx(uint8_t *buffer) {
    for (size_t i = 0; i < _out_messages_num; i++) {
        gcs_message_s *_msg_s = &_out_messages[i];
        time_period_t _now = millis();
        if (_now - _msg_s->last_sent_time >= _msg_s->period) {
            _msg_s->last_sent_time = _now;
            mavlink_message_t msg;

            if (_msg_s->hook) {
                _msg_s->hook(&msg);
                // It is very unlikely to have multiple
                // messages pending at the same time
            }
            return mavlink_msg_to_send_buffer(buffer, &msg);            
        }
    }
    return -1;
}    

void gcs_set_in_hook(uint8_t id, void (*hook)(mavlink_message_t *msg)) {
    for (size_t i = 0; i < _in_messages_num; i++) {
        gcs_message_s *msg = &_in_messages[i];
        if (msg->id == id) {
            msg->hook = hook;
            break;
        }
    }
}

void gcs_set_out_hook(uint8_t id, void (*hook)(mavlink_message_t *msg)) {
    for (size_t i = 0; i < _out_messages_num; i++) {
        gcs_message_s *msg = &_out_messages[i];
        if (msg->id == id) {
            msg->hook = hook;
            break;
        }
    }
}
