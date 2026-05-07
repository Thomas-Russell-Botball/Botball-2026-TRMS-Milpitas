#include <kipr/wombat.h>
#include <stdlib.h>

// Motor ports
#define RIGHT_BACK_MOTOR 0
#define RIGHT_FRONT_MOTOR 1
#define LEFT_FRONT_MOTOR 2
#define LEFT_BACK_MOTOR 3

// Servo ports
#define SERVO_FRONT_RIGHT 2
#define SERVO_FRONT_LEFT 3

#define START_LIGHT_SENSOR 3
#define SQUARE_LEFT_SENSOR 2
#define LINE_SENSOR 1
#define SQUARE_RIGHT_SENSOR 0

// Line following constants
#define LEFT_THRESHOLD 330
#define RIGHT_THRESHOLD 3680
#define BASE_SPEED 750

// ===================== MOVEMENT FUNCTIONS =====================

void stop() {
    mav(RIGHT_BACK_MOTOR, 0);
    mav(RIGHT_FRONT_MOTOR, 0);
    mav(LEFT_FRONT_MOTOR, 0);
    mav(LEFT_BACK_MOTOR, 0);
}

void forward(int speed, int time) {
    mav(RIGHT_BACK_MOTOR,   speed);
    mav(RIGHT_FRONT_MOTOR, -speed);
    mav(LEFT_FRONT_MOTOR,   speed);
    mav(LEFT_BACK_MOTOR,   -speed);
    msleep(time);
}

void backward(int speed, int time) {
    mav(RIGHT_BACK_MOTOR,  -speed);
    mav(RIGHT_FRONT_MOTOR,  speed);
    mav(LEFT_FRONT_MOTOR,  -speed);
    mav(LEFT_BACK_MOTOR,    speed);
    msleep(time);
}

void right(int speed, int time) {
    mav(RIGHT_BACK_MOTOR,  -speed);
    mav(RIGHT_FRONT_MOTOR,  speed);
    mav(LEFT_FRONT_MOTOR,   speed);
    mav(LEFT_BACK_MOTOR,   -speed);
    msleep(time);
}

void left(int speed, int time) {
    mav(RIGHT_BACK_MOTOR,   speed);
    mav(RIGHT_FRONT_MOTOR, -speed);
    mav(LEFT_FRONT_MOTOR,  -speed);
    mav(LEFT_BACK_MOTOR,    speed);
    msleep(time);
}

void uturn(int speed, int time){
    mav(RIGHT_BACK_MOTOR,  -speed);
    mav(RIGHT_FRONT_MOTOR,  speed);
    mav(LEFT_FRONT_MOTOR,   speed);
    mav(LEFT_BACK_MOTOR,   -speed);
    msleep(time);
}

// Line Follow Function
void line_follow(int duration_ms) {
    int elapsed = 0;
    while (elapsed < duration_ms) {
        int sensor = analog(LINE_SENSOR);

        if (sensor < LEFT_THRESHOLD) {           // Left on black
            mav(RIGHT_BACK_MOTOR,   BASE_SPEED);
            mav(RIGHT_FRONT_MOTOR, -BASE_SPEED);
            mav(LEFT_FRONT_MOTOR,   BASE_SPEED * 0.5);
            mav(LEFT_BACK_MOTOR,   -BASE_SPEED * 0.5);
        }
        else if (sensor > RIGHT_THRESHOLD) {     // Right on black
            mav(RIGHT_BACK_MOTOR,   BASE_SPEED * 0.5);
            mav(RIGHT_FRONT_MOTOR, -BASE_SPEED * 0.5);
            mav(LEFT_FRONT_MOTOR,   BASE_SPEED);
            mav(LEFT_BACK_MOTOR,   -BASE_SPEED);
        }
        else {                                   // Straight on line
            mav(RIGHT_BACK_MOTOR,   BASE_SPEED);
            mav(RIGHT_FRONT_MOTOR, -BASE_SPEED);
            mav(LEFT_FRONT_MOTOR,   BASE_SPEED);
            mav(LEFT_BACK_MOTOR,   -BASE_SPEED);
        }
        msleep(10);
        elapsed += 10;
    }
    stop();
}

// ===================== DOZER =====================
void dozer(int position, int time){
    enable_servos();
    set_servo_position(SERVO_FRONT_RIGHT, position);
    set_servo_position(SERVO_FRONT_LEFT, position);
    msleep(time);
}

void dozerup(){
    dozer(2300, 900);
}

void dozerdown(){
    dozer(995, 900);
}

int main() {
    //wait_for_light(START_LIGHT_SENSOR);
    shut_down_in(118);
   
    // === START SEQUENCE ===
    dozerup();
    dozerdown();
   
    // Drive forward past first black line
    forward(80, 5700);
    msleep(300);
   
    // Turn right toward the pom/cone line
    right(100, 1150);
    msleep(250);
   
    // Follow line while scooping blue poms + cones
    line_follow(7800);
    msleep(300);
   
    // U-turn after second cone
    uturn(95, 1350);
    msleep(300);
   
    // Follow line while scooping orange poms
    line_follow(8200);
    msleep(300);
   
    // Turn left at end of board
    left(100, 1050);
    msleep(250);
   
    // Drive into Lower Start Box
    forward(70, 2800);
    msleep(300);
   
    // Semi-raise dozer to drop poms and cone
    dozer(1650, 600);
    msleep(400);
   
    // === KNOCK DOWN 3-CUBE TOWER ===
    forward(100, 850);
    msleep(200);
   
    dozer(1950, 500);
    msleep(300);
   
    left(80, 450);
    msleep(200);
   
    backward(80, 650);
    msleep(200);
   
    forward(90, 950);
   
    // Final park position
    dozerup();
    stop();
    disable_servos();
   
    return 0;
}
