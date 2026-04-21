#ifndef WINNIN_NAVIGATION_H
#define WINNIN_NAVIGATION_H

#include "motor_control.h"
#include "sensors.h"

static inline void drive_straight_ticks(int left_forward_motor, int left_reverse_motor, int right_forward_motor, int right_reverse_motor, int ticks, int speed)
{
    smooth_drive_ticks(left_forward_motor, left_reverse_motor, right_forward_motor, right_reverse_motor, ticks, speed, 35, 10);
}

static inline void turn_arc_ticks(int left_forward_motor, int left_reverse_motor, int right_forward_motor, int right_reverse_motor, int ticks, int speed, float inside_wheel_factor)
{
    int direction = (ticks >= 0) ? 1 : -1;
    int abs_ticks = ticks * direction;
    int outside_speed = speed;
    int inside_speed = (int)(speed * inside_wheel_factor);

    if (inside_speed < 0) {
        inside_speed = -inside_speed;
    }

    cmpc(left_forward_motor);
    cmpc(right_forward_motor);

    while (1) {
        int progress = get_motor_position_counter(right_forward_motor) * direction;
        if (progress >= abs_ticks) {
            break;
        }

        set_drive_velocity(
            left_forward_motor,
            left_reverse_motor,
            right_forward_motor,
            right_reverse_motor,
            direction * inside_speed,
            direction * outside_speed
        );
        msleep(10);
    }

    stop_drive(left_forward_motor, left_reverse_motor, right_forward_motor, right_reverse_motor);
}

static inline void square_up_dual_sensors(int left_forward_motor, int left_reverse_motor, int right_forward_motor, int right_reverse_motor, int left_sensor_port, int right_sensor_port, int threshold, int settle_cycles)
{
    int stable_cycles = 0;
    int left_speed = 350;
    int right_speed = 350;

    if (settle_cycles <= 0) {
        settle_cycles = 4;
    }

    while (stable_cycles < settle_cycles) {
        int left_on_line = sensor_is_on_line(left_sensor_port, threshold);
        int right_on_line = sensor_is_on_line(right_sensor_port, threshold);

        left_speed = left_on_line ? -120 : 350;
        right_speed = right_on_line ? -120 : 350;
        set_drive_velocity(left_forward_motor, left_reverse_motor, right_forward_motor, right_reverse_motor, left_speed, right_speed);

        if (left_on_line && right_on_line) {
            stable_cycles += 1;
        } else {
            stable_cycles = 0;
        }
        msleep(25);
    }

    stop_drive(left_forward_motor, left_reverse_motor, right_forward_motor, right_reverse_motor);
}

#endif
