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

// OLD NOT WORKING VARS (kept for compatibility)
#define LINE_TARGET 3000
#define BLACK_VALUE 1300
#define LEFT_TOPHAT 3
#define RIGHT_TOPHAT 2
#define LEFT_THRESHOLD 330
#define RIGHT_THRESHOLD 3680
#define BASE_SPEED 2000
#define TURN_FACTOR 1
#define LEFT_TURN_FACTOR 0.7

// ===================== MAV VERSION OF MOVEMENT =====================

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

void right(int speed, int time) { // turn right
    mav(RIGHT_BACK_MOTOR,  -speed);
    mav(RIGHT_FRONT_MOTOR,  speed);
    mav(LEFT_FRONT_MOTOR,   speed);
    mav(LEFT_BACK_MOTOR,   -speed);
    msleep(time);
}

void left(int speed, int time) { // turn left
    mav(RIGHT_BACK_MOTOR,   speed);
    mav(RIGHT_FRONT_MOTOR, -speed);
    mav(LEFT_FRONT_MOTOR,  -speed);
    mav(LEFT_BACK_MOTOR,    speed);
    msleep(time);
}

void uturn(int speed, int time){
    mav(RIGHT_BACK_MOTOR,  -speed);   // Fixed polarity for consistent turn
    mav(RIGHT_FRONT_MOTOR,  speed);
    mav(LEFT_FRONT_MOTOR,   speed);
    mav(LEFT_BACK_MOTOR,   -speed);
    msleep(time);
}

void dozer(int position, int time){
    enable_servos();
    set_servo_position(SERVO_FRONT_RIGHT, position);
    set_servo_position(SERVO_FRONT_LEFT, position);
    msleep(time);
}

void dozerup(){
    enable_servos();
    dozer(2300, 900);
}

void dozerdown(){
    enable_servos();
    dozer(995, 900);
}

void stop() {
    mav(RIGHT_BACK_MOTOR, 0);
    mav(RIGHT_FRONT_MOTOR, 0);
    mav(LEFT_FRONT_MOTOR, 0);
    mav(LEFT_BACK_MOTOR, 0);
}

int main() {
    //wait_for_light(START_LIGHT_SENSOR);   // Uncomment for tournament
    shut_down_in(118);
   
    // === START SEQUENCE ===
    dozerup();           // Raise bulldozer once
    dozerdown();         // Lower to scooping position
   
    // Drive forward past first black line
    forward(80, 5700);
    msleep(300);
   
    // Turn right toward the pom/cone line
    right(100, 1150);
    msleep(250);
   
    // Drive forward scooping blue poms + cones
    forward(75, 6200);
    msleep(300);
   
    // U-turn after grabbing second cone
    uturn(95, 1350);
    msleep(300);
   
    // Continue on line scooping orange poms
    forward(75, 6300);
    msleep(300);
   
    // Turn left at end of board
    left(100, 1050);
    msleep(250);
   
    // Drive into Lower Start Box
    forward(70, 2400);
    msleep(300);
   
    // Semi-raise dozer to drop poms and cone
    dozer(1650, 600);
    msleep(400);
   
    // === KNOCK DOWN 3-CUBE TOWER ===
    forward(100, 850);     // Ram into front of box
    msleep(200);
   
    dozer(1950, 500);      // Raise dozer a bit
    msleep(300);
   
    left(80, 450);         // Slight left turn to knock tower
    msleep(200);
   
    backward(80, 650);     // Back up
    msleep(200);
   
    forward(90, 950);      // Final ram to square up
   
    // Final park position
    dozerup();
    stop();
    disable_servos();
   
    return 0;
}
