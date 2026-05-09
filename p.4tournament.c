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
    dozer(1170, 900);
}

int main() {
    wait_for_light(5);
    shut_down_in(118);
   
    // === START SEQUENCE ===
    dozerup();
    msleep(300);
    dozerdown();
    msleep(300);
   
    // Drive forward past first black line
    forward(900, 2600);
    msleep(320);
    
    // Turn right toward the pom/cone line #1
    right(800, 1350); //1650
    msleep(300);
    
    // Drive 1
    forward(1300, 100);
    msleep(300);

    // Right 2
    right(800, 3); //1650
    msleep(300);
    
    // Drive 2
    forward(1300, 100);
    msleep(300);
    
    // Right 3
    right(800, 50); //1650
    msleep(300);
    
    // Drive 3
    forward(1300, 1000);
    msleep(300);

    right(800, 20); //1650
    msleep(300);
    
    // Drive 4
    forward(1300, 500);
    msleep(300);
    
    // Right 4
    right(800, 5); //1650
    msleep(200);

    // Drive forward scooping blue poms + cones
    forward(850, 1000);
    msleep(300);
    
    // Right 5
    //right(800, 5); //1650
    //msleep(100);

    forward(850, 4000);
    msleep(300);
    
    right(800, 2); //1650
    msleep(200);
    
    //right(800, 15); //1650
    //msleep(300);

    forward(850, 4300);
    msleep(350);
    
    /////////// BLUE DONE

    //right(800, 100);
    //msleep(200);

    //ao();
    //msleep(2000); ////////////
    
    forward(1050, 2000);
    msleep(500);

    right(800, 450);
    msleep(300);
    forward(800, 500);
    msleep(200);
    right(800, 1090);
    msleep(1290);
   
    // UTURN DONE, NOW TIME FOR THE ORANGE ONES
    forward(850, 3300);
    msleep(2200);

    right(800, 250);
    
    forward(850, 1500);
    
    right(800, 150);

    right(800, 200); 

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
    
    //Placeholder for Extra Stuff
    
    // DONE :D
    
    dozerup();
    msleep(400);

    dozerdown();
    stop();
    disable_servos();
   
    return 0;
}
