#include <kipr/wombat.h>
#include <stdlib.h>

// Motor ports (your latest)
#define RIGHT_BACK_MOTOR 0
#define RIGHT_FRONT_MOTOR 1
#define LEFT_FRONT_MOTOR 2
#define LEFT_BACK_MOTOR 3

// Servo ports (your original)
#define SERVO_FRONT_RIGHT 2
#define SERVO_FRONT_LEFT 3

#define START_LIGHT_SENSOR 3
#define SQUARE_LEFT_SENSOR 2
#define LINE_SENSOR 1
#define SQUARE_RIGHT_SENSOR 0

void stop() {
    mav(RIGHT_BACK_MOTOR, 0);
    mav(RIGHT_FRONT_MOTOR, 0);
    mav(LEFT_FRONT_MOTOR, 0);
    mav(LEFT_BACK_MOTOR, 0);
}

void forward(int speed, int time) {
    mav(RIGHT_BACK_MOTOR,  speed);
    mav(RIGHT_FRONT_MOTOR, -speed);
    mav(LEFT_FRONT_MOTOR,  speed);
    mav(LEFT_BACK_MOTOR,   -speed);
    msleep(time);
}

void backward(int speed, int time) {
    mav(RIGHT_BACK_MOTOR,  -speed);
    mav(RIGHT_FRONT_MOTOR, speed);
    mav(LEFT_FRONT_MOTOR,  -speed);
    mav(LEFT_BACK_MOTOR,   speed);
    msleep(time);
}

void right(int speed, int time) {
    mav(RIGHT_BACK_MOTOR,  -speed);
    mav(RIGHT_FRONT_MOTOR, speed);
    mav(LEFT_FRONT_MOTOR, speed);
    mav(LEFT_BACK_MOTOR,  -speed);
    msleep(time);
}

void left(int speed, int time) {
    mav(RIGHT_BACK_MOTOR, speed);
    mav(RIGHT_FRONT_MOTOR, -speed);
    mav(LEFT_FRONT_MOTOR,  -speed);
    mav(LEFT_BACK_MOTOR,   speed);
    msleep(time);
}

void uturn(int speed, int time){
    mav(RIGHT_BACK_MOTOR, 0);
    mav(RIGHT_FRONT_MOTOR, 0);
    mav(LEFT_FRONT_MOTOR, speed);
    mav(LEFT_BACK_MOTOR, speed);
    msleep(time);
}

// Dozer
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
    dozer(1225, 900);
}

int main() {
    //wait_for_light(START_LIGHT_SENSOR);
    shut_down_in(118);
   
    // === START SEQUENCE ===
    dozerup();
    msleep(300);
    dozerdown();
    msleep(300);
   
    // Drive forward past first black line
    forward(900, 3000);
    msleep(330);
   
    // Turn right toward the pom/cone line
    right(800, 400);
    msleep(300);

    forward(450, 450);
    msleep(200);

    right(800, 100);
    msleep(300);

    forward(1300, 1000);
    msleep(300);
    
    right(800, 100);
    msleep(300);
    
    forward(1300, 500);
    msleep(300);

    right(800, 250);
    msleep(300);

    // Drive forward scooping blue poms + cones
    forward(850, 1000);
    msleep(300);

    right(800, 225);
    msleep(300);

    forward(850, 4200);
    msleep(300);

    forward(850, 4200);
    msleep(300);

    right(800, 100);
    msleep(200);

    forward(1050, 3950);
    msleep(700);

    right(800, 450);
    msleep(300);
    forward(800, 500);
    msleep(200);
    right(800, 2600);
    msleep(1300);
    stop();

    ao();
    msleep(2000);
   
    // Return / rest of sequence
    forward(850, 3000);
    msleep(500);

    right(800, 200);

    forward(900, 7500);
    msleep(500);

    right(800, 400); 

    forward(900, 7500);
    msleep(7000);

    // Turn left at end of board
    left(800, 1050);
    msleep(500);
   
    // Drive into Lower Start Box
    forward(800, 5000);
    msleep(5000);
   
    // Semi-raise dozer to drop poms and cone
    dozer(1650, 600);
    msleep(400);

    forward(800, 2000);
    msleep(800);
   
    // === KNOCK DOWN 3-CUBE TOWER ===
    backward(900, 4550);
    msleep(4000);

    dozerdown();

    left(1200, 2400);
    msleep(1200);

    ao();
    msleep(1000);

    forward(850, 4200);
    msleep(800);

    right(850, 700);
    msleep(600);

    forward(850, 2000);
    msleep(600);

    right(850, 300);
    msleep(600);
   
    forward(850, 3200);
    msleep(400);
    left(800, 1050);
    msleep(500);
   
    forward(800, 2400);
    msleep(300);
   
    dozer(1650, 600);
    dozerup();
    msleep(400);

    dozerdown();
    stop();
    disable_servos();
   
    return 0;
}
