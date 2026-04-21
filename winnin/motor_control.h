#ifndef WINNIN_MOTOR_CONTROL_H
#define WINNIN_MOTOR_CONTROL_H

#include <kipr/wombat.h>

static inline int clamp_speed(int speed, int max_abs_speed)
{
    if (speed > max_abs_speed) {
        return max_abs_speed;
    }
    if (speed < -max_abs_speed) {
        return -max_abs_speed;
    }
    return speed;
}

static inline void set_drive_velocity(int left_forward_motor, int left_reverse_motor, int right_forward_motor, int right_reverse_motor, int left_speed, int right_speed)
{
    mav(left_forward_motor, left_speed);
    mav(left_reverse_motor, -left_speed);
    mav(right_forward_motor, right_speed);
    mav(right_reverse_motor, -right_speed);
}

static inline void stop_drive(int left_forward_motor, int left_reverse_motor, int right_forward_motor, int right_reverse_motor)
{
    set_drive_velocity(left_forward_motor, left_reverse_motor, right_forward_motor, right_reverse_motor, 0, 0);
    ao();
    msleep(20);
}

static inline void smooth_drive_ticks(int left_forward_motor, int left_reverse_motor, int right_forward_motor, int right_reverse_motor, int target_ticks, int target_speed, int accel_step, int accel_delay_ms)
{
    int direction = (target_ticks >= 0) ? 1 : -1;
    int speed = 0;
    int abs_ticks = target_ticks * direction;
    int abs_target_speed = target_speed * direction;

    if (abs_target_speed < 0) {
        abs_target_speed = -abs_target_speed;
    }
    if (accel_step <= 0) {
        accel_step = 25;
    }
    if (accel_delay_ms <= 0) {
        accel_delay_ms = 10;
    }

    cmpc(left_forward_motor);
    cmpc(right_forward_motor);

    while (1) {
        int avg_ticks = (get_motor_position_counter(left_forward_motor) + get_motor_position_counter(right_forward_motor)) / 2;
        int progress = avg_ticks * direction;
        int remaining = abs_ticks - progress;
        int decel_limit = remaining * 3;

        if (remaining <= 0) {
            break;
        }

        if (speed < abs_target_speed) {
            speed += accel_step;
            if (speed > abs_target_speed) {
                speed = abs_target_speed;
            }
        }

        if (decel_limit < speed) {
            speed = decel_limit;
            if (speed < 120) {
                speed = 120;
            }
        }

        set_drive_velocity(left_forward_motor, left_reverse_motor, right_forward_motor, right_reverse_motor, direction * speed, direction * speed);
        msleep(accel_delay_ms);
    }

    stop_drive(left_forward_motor, left_reverse_motor, right_forward_motor, right_reverse_motor);
}

#endif
