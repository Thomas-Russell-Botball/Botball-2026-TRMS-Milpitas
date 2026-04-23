#include <kipr/wombat.h>
#include <stdlib.h>
#include <math.h>

/*
  Tournament code for KIPR Wombat
  Single-file, modular-style functions
  Inspired by proven Rancho patterns, but cleaned up and made safer.
*/

/* ----------------------- CONFIG ----------------------- */

// Drive motors (adjust if your wiring differs)
#define LM 1
#define RM 3

// Sensors
#define LINE_RIGHT 3   // analog right line sensor
#define LINE_LEFT  5   // analog left line sensor

// Servos
#define ARM_SERVO  2
#define CLAW_SERVO 1

// Servo positions (tune on robot)
#define ARM_DOWN   520
#define ARM_UP     900
#define CLAW_OPEN  1200
#define CLAW_CLOSE 450

// Line tuning
#define LINE_KP            0.50f
#define LINE_TARGET        3200
#define SQUARE_GRAY        3230   // midpoint-ish between white/black
#define SQUARE_BACK_FACTOR 0.60f

// Utility clamps
static int clampi(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* ------------------- LOW-LEVEL MOTION ------------------- */

void drive_tank(int left, int right) {
    mav(LM, left);
    mav(RM, right);
}

void stop_drive(void) {
    mav(LM, 0);
    mav(RM, 0);
    msleep(30);
}

void drive_ms(int left, int right, int ms) {
    drive_tank(left, right);
    msleep(ms);
    stop_drive();
}

/* Smooth servo move */
void servo_smooth(int port, int from, int to, int step, int delay_ms) {
    if (step <= 0) step = 10;

    if (from < to) {
        for (int p = from; p <= to; p += step) {
            set_servo_position(port, p);
            msleep(delay_ms);
        }
    } else {
        for (int p = from; p >= to; p -= step) {
            set_servo_position(port, p);
            msleep(delay_ms);
        }
    }
    set_servo_position(port, to);
}

/* Encoder-based pivot turn */
void turn_encoder(int ticks, int speed) {
    cmpc(LM);
    while (abs(gmpc(LM)) < abs(ticks)) {
        int s = (ticks >= 0) ? speed : -speed;
        // opposite directions for pivot
        drive_tank(s, s);
    }
    stop_drive();
}

/* ------------------- SENSOR-BASED CONTROL ------------------- */

/* Single-sensor proportional line follow */
void line_follow_ticks(int ticks, int base_speed, int use_right_sensor) {
    cmpc(LM);

    while (abs(gmpc(LM)) < abs(ticks)) {
        int sensor = use_right_sensor ? analog(LINE_RIGHT) : analog(LINE_LEFT);
        int error = sensor - LINE_TARGET;
        float correction_f = LINE_KP * error;
        int correction = (int)correction_f;

        int l = base_speed - correction;
        int r = -base_speed - correction;  // RM often inverted on many bots
        l = clampi(l, -1500, 1500);
        r = clampi(r, -1500, 1500);

        drive_tank(l, r);
    }
    stop_drive();
}

/* Dual-sensor square up against a black line */
void square_up(int speed) {
    while (analog(LINE_RIGHT) < SQUARE_GRAY || analog(LINE_LEFT) < SQUARE_GRAY) {
        // right sensor control -> LM
        if (analog(LINE_RIGHT) < SQUARE_GRAY) {
            mav(LM, speed);
        } else {
            mav(LM, (int)(-speed * SQUARE_BACK_FACTOR));
        }

        // left sensor control -> RM
        if (analog(LINE_LEFT) < SQUARE_GRAY) {
            mav(RM, -speed);
        } else {
            mav(RM, (int)(speed * SQUARE_BACK_FACTOR));
        }
    }
    stop_drive();
}

/* ------------------- GAME ACTIONS ------------------- */

void prep_start_box(void) {
    enable_servos();

    // open claw + arm low enough to fit, then up for drive
    set_servo_position(CLAW_SERVO, CLAW_OPEN);
    msleep(500);

    set_servo_position(ARM_SERVO, ARM_DOWN);
    msleep(700);

    servo_smooth(ARM_SERVO, ARM_DOWN, ARM_UP, 20, 35);
}

void grab_object(void) {
    // lower arm, close claw, raise arm
    servo_smooth(ARM_SERVO, ARM_UP, 250, 20, 35);
    set_servo_position(CLAW_SERVO, CLAW_CLOSE);
    msleep(500);
    servo_smooth(ARM_SERVO, 250, ARM_UP, 20, 35);
}

void release_object(void) {
    set_servo_position(CLAW_SERVO, CLAW_OPEN - 150);
    msleep(500);
}

/* ------------------- MAIN RUN PLAN ------------------- */

int main(void) {
    // Competition-safe startup
    wait_for_light(0);
    shut_down_in(119);

    prep_start_box();

    // Phase 1: leave start / orient to board
    drive_ms(700, -700, 1900);
    turn_encoder(800, 1200);
    drive_ms(900, -900, 5000);
    turn_encoder(-1000, 1200);
    drive_ms(-800, 800, 1100);

    // Phase 2: line capture + traverse
    square_up(700);
    line_follow_ticks(3000, 1000, 1);  // right sensor follow
    square_up(700);
    line_follow_ticks(3200, 600, 1);

    // Phase 3: approach and grab
    stop_drive();
    msleep(300);
    turn_encoder(1000, 1200);
    drive_ms(-500, 500, 650);
    grab_object();

    // Phase 4: deliver
    turn_encoder(2000, 1200);
    drive_ms(500, -500, 700);
    release_object();

    stop_drive();
    disable_servos();
    return 0;
}
