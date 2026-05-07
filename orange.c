#include <kipr/wombat.h>

// Motor ports (your original)
#define RIGHT_BACK_MOTOR 0
#define RIGHT_FRONT_MOTOR 1
#define LEFT_FRONT_MOTOR 2
#define LEFT_BACK_MOTOR 3

// Servo ports (your original)
#define SERVO_FRONT_RIGHT 2
#define SERVO_FRONT_LEFT 3

#define START_LIGHT_SENSOR 3

// ===================== ROBUST ENCODER MOVEMENT =====================

void stop() {
    mav(RIGHT_BACK_MOTOR, 0);
    mav(RIGHT_FRONT_MOTOR, 0);
    mav(LEFT_FRONT_MOTOR, 0);
    mav(LEFT_BACK_MOTOR, 0);
}

void move_forward_ticks(int speed, int ticks) {
    cmcp(RIGHT_BACK_MOTOR);
    cmcp(RIGHT_FRONT_MOTOR);
    cmcp(LEFT_FRONT_MOTOR);
    cmcp(LEFT_BACK_MOTOR);
    
    while (gmcp(LEFT_FRONT_MOTOR) < ticks) {
        mav(RIGHT_BACK_MOTOR,   speed);
        mav(RIGHT_FRONT_MOTOR, -speed);
        mav(LEFT_FRONT_MOTOR,   speed);
        mav(LEFT_BACK_MOTOR,   -speed);
    }
    stop();
}

void move_backward_ticks(int speed, int ticks) {
    cmcp(RIGHT_BACK_MOTOR);
    cmcp(RIGHT_FRONT_MOTOR);
    cmcp(LEFT_FRONT_MOTOR);
    cmcp(LEFT_BACK_MOTOR);
    
    int target = -ticks;
    while (gmcp(LEFT_FRONT_MOTOR) > target) {
        mav(RIGHT_BACK_MOTOR,  -speed);
        mav(RIGHT_FRONT_MOTOR,  speed);
        mav(LEFT_FRONT_MOTOR,  -speed);
        mav(LEFT_BACK_MOTOR,    speed);
    }
    stop();
}

void right_ticks(int speed, int ticks) {     // Turn Right
    cmcp(RIGHT_BACK_MOTOR);
    cmcp(RIGHT_FRONT_MOTOR);
    cmcp(LEFT_FRONT_MOTOR);
    cmcp(LEFT_BACK_MOTOR);
    
    while (gmcp(LEFT_FRONT_MOTOR) < ticks) {
        mav(RIGHT_BACK_MOTOR,  -speed);
        mav(RIGHT_FRONT_MOTOR,  speed);
        mav(LEFT_FRONT_MOTOR,   speed);
        mav(LEFT_BACK_MOTOR,   -speed);
    }
    stop();
}

void left_ticks(int speed, int ticks) {      // Turn Left
    cmcp(RIGHT_BACK_MOTOR);
    cmcp(RIGHT_FRONT_MOTOR);
    cmcp(LEFT_FRONT_MOTOR);
    cmcp(LEFT_BACK_MOTOR);
    
    while (gmcp(LEFT_FRONT_MOTOR) > -ticks) {
        mav(RIGHT_BACK_MOTOR,   speed);
        mav(RIGHT_FRONT_MOTOR, -speed);
        mav(LEFT_FRONT_MOTOR,  -speed);
        mav(LEFT_BACK_MOTOR,    speed);
    }
    stop();
}

void uturn_ticks(int speed, int ticks) {
    cmcp(RIGHT_BACK_MOTOR);
    cmcp(RIGHT_FRONT_MOTOR);
    cmcp(LEFT_FRONT_MOTOR);
    cmcp(LEFT_BACK_MOTOR);
    
    while (gmcp(LEFT_FRONT_MOTOR) < ticks) {
        mav(RIGHT_BACK_MOTOR,  -speed);
        mav(RIGHT_FRONT_MOTOR,  speed);
        mav(LEFT_FRONT_MOTOR,   speed);
        mav(LEFT_BACK_MOTOR,   -speed);
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
    dozer(975, 900);
}

int main() {
    //wait_for_light(START_LIGHT_SENSOR);   // Uncomment for tournament
    shut_down_in(118);
    
    // === START SEQUENCE ===
    dozerup();           // Raise bulldozer
    dozerdown();         // Lower to scooping position
    
    // === MAIN ROUTINE - Conservative speeds for old batteries ===
    move_forward_ticks(750, 7200);     // Forward past first line
    msleep(400);
    
    right_ticks(650, 1550);            // Turn right
    msleep(300);
    
    move_forward_ticks(700, 7600);     // Scoop blue poms + cones
    msleep(400);
    
    uturn_ticks(700, 2050);            // U-turn
    msleep(400);
    
    move_forward_ticks(700, 8100);     // Scoop orange poms
    msleep(400);
    
    left_ticks(650, 1420);             // Turn left
    msleep(300);
    
    move_forward_ticks(700, 3900);     // Into Lower Start Box
    msleep(400);
    
    // Drop poms & cone
    dozer(1650, 800);
    msleep(600);
    
    // === KNOCK DOWN 3-CUBE TOWER ===
    move_forward_ticks(850, 1000);     // Ram front wall
    msleep(300);
    
    dozer(1950, 600);
    msleep(400);
    
    left_ticks(550, 580);              // Slight left knock
    msleep(300);
    
    move_backward_ticks(700, 750);
    msleep(300);
    
    move_forward_ticks(850, 1150);     // Final square up
    
    // Final park
    dozerup();
    stop();
    disable_servos();
    
    return 0;
}
