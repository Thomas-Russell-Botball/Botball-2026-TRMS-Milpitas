#ifndef WINNIN_LINE_FOLLOWING_H
#define WINNIN_LINE_FOLLOWING_H

#include "motor_control.h"
#include "sensors.h"

typedef struct LineFollowConfig {
    int target_value;
    float compensation_rate;
    int max_correction;
    int line_lost_margin;
} LineFollowConfig;

static inline int line_error(int sensor_port, int target_value)
{
    return read_analog_sensor(sensor_port) - target_value;
}

static inline int line_follow_ticks(int left_forward_motor, int left_reverse_motor, int right_forward_motor, int right_reverse_motor, int tracker_sensor_port, int distance_ticks, int base_speed, LineFollowConfig config)
{
    int direction = (distance_ticks >= 0) ? 1 : -1;
    int abs_ticks = distance_ticks * direction;
    int line_lost_count = 0;

    cmpc(left_forward_motor);
    cmpc(right_forward_motor);

    while (1) {
        int avg_ticks = (get_motor_position_counter(left_forward_motor) + get_motor_position_counter(right_forward_motor)) / 2;
        int progress = avg_ticks * direction;
        int sensor_value = read_analog_sensor(tracker_sensor_port);
        int error = sensor_value - config.target_value;
        int correction = (int)(-error * config.compensation_rate);

        if (progress >= abs_ticks) {
            break;
        }

        if (correction > config.max_correction) {
            correction = config.max_correction;
        } else if (correction < -config.max_correction) {
            correction = -config.max_correction;
        }

        if (error > config.line_lost_margin || error < -config.line_lost_margin) {
            line_lost_count += 1;
        } else {
            line_lost_count = 0;
        }

        set_drive_velocity(
            left_forward_motor,
            left_reverse_motor,
            right_forward_motor,
            right_reverse_motor,
            direction * (base_speed - correction),
            direction * (base_speed + correction)
        );
        msleep(10);
    }

    stop_drive(left_forward_motor, left_reverse_motor, right_forward_motor, right_reverse_motor);
    return line_lost_count;
}

#endif
