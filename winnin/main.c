#include <kipr/wombat.h>

#include "line_following.h"
#include "navigation.h"
#include "servo_control.h"

#define LEFT_MOTOR_FORWARD 0
#define LEFT_MOTOR_REVERSE 1
#define RIGHT_MOTOR_FORWARD 2
#define RIGHT_MOTOR_REVERSE 3

#define LINE_SENSOR_LEFT 0
#define LINE_SENSOR_RIGHT 1

#define CLAW_SERVO 0
#define ARM_SERVO 2

static void phase_red_pickup(void)
{
    move_servo_smooth(ARM_SERVO, 1900, 8, 8);
    move_servo_smooth(CLAW_SERVO, 1700, 8, 8);
    drive_straight_ticks(LEFT_MOTOR_FORWARD, LEFT_MOTOR_REVERSE, RIGHT_MOTOR_FORWARD, RIGHT_MOTOR_REVERSE, 1800, 1100);
    move_servo_smooth(CLAW_SERVO, 700, 8, 8);
    msleep(120);
}

static void phase_orange_drop(void)
{
    turn_arc_ticks(LEFT_MOTOR_FORWARD, LEFT_MOTOR_REVERSE, RIGHT_MOTOR_FORWARD, RIGHT_MOTOR_REVERSE, 800, 950, 0.55f);
    drive_straight_ticks(LEFT_MOTOR_FORWARD, LEFT_MOTOR_REVERSE, RIGHT_MOTOR_FORWARD, RIGHT_MOTOR_REVERSE, 2400, 1200);
    move_servo_smooth(ARM_SERVO, 700, 8, 8);
    move_servo_smooth(CLAW_SERVO, 1800, 8, 8);
}

static void phase_blue_wall_localize(void)
{
    drive_straight_ticks(LEFT_MOTOR_FORWARD, LEFT_MOTOR_REVERSE, RIGHT_MOTOR_FORWARD, RIGHT_MOTOR_REVERSE, -800, 900);
    square_up_dual_sensors(LEFT_MOTOR_FORWARD, LEFT_MOTOR_REVERSE, RIGHT_MOTOR_FORWARD, RIGHT_MOTOR_REVERSE, LINE_SENSOR_LEFT, LINE_SENSOR_RIGHT, 2200, 5);
}

static void phase_green_cube_grab(void)
{
    LineFollowConfig config;
    int line_health = 0;

    config.target_value = 3000;
    config.compensation_rate = 0.20f;
    config.max_correction = 700;
    config.line_lost_margin = 1400;

    line_health = line_follow_ticks(
        LEFT_MOTOR_FORWARD,
        LEFT_MOTOR_REVERSE,
        RIGHT_MOTOR_FORWARD,
        RIGHT_MOTOR_REVERSE,
        LINE_SENSOR_LEFT,
        2600,
        1000,
        config
    );

    if (line_health > 20) {
        square_up_dual_sensors(LEFT_MOTOR_FORWARD, LEFT_MOTOR_REVERSE, RIGHT_MOTOR_FORWARD, RIGHT_MOTOR_REVERSE, LINE_SENSOR_LEFT, LINE_SENSOR_RIGHT, 2200, 4);
    }

    move_servo_smooth(ARM_SERVO, 1700, 8, 8);
    move_servo_smooth(CLAW_SERVO, 750, 8, 8);
}

int main(void)
{
    enable_servos();
    set_servo_position(CLAW_SERVO, 1800);
    set_servo_position(ARM_SERVO, 700);
    msleep(250);

    wait_for_light(5);
    msleep(1200);

    phase_red_pickup();
    phase_orange_drop();
    phase_blue_wall_localize();
    phase_green_cube_grab();

    stop_drive(LEFT_MOTOR_FORWARD, LEFT_MOTOR_REVERSE, RIGHT_MOTOR_FORWARD, RIGHT_MOTOR_REVERSE);
    disable_servos();
    return 0;
}
