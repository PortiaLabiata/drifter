#include "gcs.h"

static gcs_s _gcs;

const size_t _out_messages_num = 1;
const size_t _in_messages_num = 1;

gcs_message_s _out_messages[] = {
    {.period = 1000, .last_sent_time = 0, .id = 0, .hook = NULL},
};

gcs_message_s _in_messages[] = {
    {.period = 0, .last_sent_time = 0, .id = 0, .hook = NULL},
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
