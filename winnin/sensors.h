#ifndef WINNIN_SENSORS_H
#define WINNIN_SENSORS_H

#include <kipr/wombat.h>

static inline int read_analog_sensor(int port)
{
    return analog(port);
}

static inline int read_digital_sensor(int port)
{
    return digital(port);
}

static inline int sensor_is_on_line(int port, int threshold)
{
    return read_analog_sensor(port) > threshold;
}

static inline int dual_sensors_ready(int left_port, int right_port, int threshold)
{
    return sensor_is_on_line(left_port, threshold) && sensor_is_on_line(right_port, threshold);
}

#endif
