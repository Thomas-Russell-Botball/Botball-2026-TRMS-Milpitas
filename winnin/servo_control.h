#ifndef WINNIN_SERVO_CONTROL_H
#define WINNIN_SERVO_CONTROL_H

#include <kipr/wombat.h>

static inline void move_servo_smooth(int port, int target_position, int step_size, int delay_ms)
{
    int current_position = get_servo_position(port);
    int direction = (target_position >= current_position) ? 1 : -1;

    if (step_size <= 0) {
        step_size = 5;
    }
    if (delay_ms <= 0) {
        delay_ms = 10;
    }

    while ((target_position - current_position) * direction > 0) {
        current_position += direction * step_size;
        if ((target_position - current_position) * direction < 0) {
            current_position = target_position;
        }
        set_servo_position(port, current_position);
        msleep(delay_ms);
    }
}

#endif
