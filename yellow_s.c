#include <kipr/wombat.h>
#include <stdlib.h>

// Motor ports (your original)
#define RIGHT_BACK_MOTOR 1
#define RIGHT_FRONT_MOTOR 3
#define LEFT_FRONT_MOTOR 0
#define LEFT_BACK_MOTOR 2

// Servo ports (your original)
#define SERVO_FRONT_RIGHT 0
#define SERVO_FRONT_LEFT 0

#define START_LIGHT_SENSOR 3
#define SQUARE_LEFT_SENSOR 2
#define LINE_SENSOR 1
#define SQUARE_RIGHT_SENSOR 0

//OLD NOT WORKING VARs
#define LINE_TARGET 3000
#define BLACK_VALUE 1300

#define LEFT_TOPHAT 3
#define RIGHT_TOPHAT 2

#define LEFT_THRESHOLD 330
#define RIGHT_THRESHOLD 3680
#define BASE_SPEED 2000
#define TURN_FACTOR 1
#define LEFT_TURN_FACTOR 0.7

void forward(int speed, int time) {
    motor(RIGHT_BACK_MOTOR,  speed);
    motor(RIGHT_FRONT_MOTOR, speed);
    motor(LEFT_FRONT_MOTOR,  speed);
    motor(LEFT_BACK_MOTOR,  speed);
    msleep(time);
}

void backward(int speed, int time) {
    motor(RIGHT_BACK_MOTOR,  -speed);
    motor(RIGHT_FRONT_MOTOR, -speed);
    motor(LEFT_FRONT_MOTOR,  -speed);
    motor(LEFT_BACK_MOTOR,   -speed);
    msleep(time);
}

void right(int speed, int time) {      // turn right
    motor(RIGHT_BACK_MOTOR,  speed);
    motor(RIGHT_FRONT_MOTOR,  speed);
    motor(LEFT_FRONT_MOTOR,   -speed);
    motor(LEFT_BACK_MOTOR,   speed);
    msleep(time);
}

void left(int speed, int time) {       // turn left
    motor(RIGHT_BACK_MOTOR,   -speed);
    motor(RIGHT_FRONT_MOTOR, -speed);
    motor(LEFT_FRONT_MOTOR,  speed);
    motor(LEFT_BACK_MOTOR,    speed);
    msleep(time);
}

void uturn(int speed, int time){
    motor(RIGHT_BACK_MOTOR,  0);
    motor(RIGHT_FRONT_MOTOR,  0);
    motor(LEFT_FRONT_MOTOR,   speed);
    motor(LEFT_BACK_MOTOR,   speed);
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
    motor(RIGHT_BACK_MOTOR, 0);
    motor(RIGHT_FRONT_MOTOR, 0);
    motor(LEFT_FRONT_MOTOR, 0);
    motor(LEFT_BACK_MOTOR, 0);
}

int main() {
    //wait_for_light(5);
    shut_down_in(118);
    
    // === START SEQUENCE ===
    //dozerup();                    // Raise bulldozer once
    dozerdown();                  // Lower to scooping position
    
		//motor(0,1000);
		//motor(3,-1000);
		//msleep(400);

		//forward(100, 200);
		//msleep(300);

		//motor(0,1000);
		//motor(3,-1000);
		//msleep(300);

    // Drive forward past first black line
    forward(80, 4500);
    msleep(330);
    
		//stop();
		//msleep(10000);
    // Turn right toward the pom/cone line
    right(100, 800);
    msleep(250);
    
    // Drive forward scooping blue poms + cones
    forward(75, 6200);
    msleep(300);
    
    // U-turn after grabbing second cone
    right(100, 450);
    msleep(300);

		forward(75,1000);
		msleep(300);

		right(100, 450);
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
    forward(100, 850);      // Ram into front of box
    msleep(200);
    
    dozer(1950, 500);       // Raise dozer a bit
    msleep(300);
    
    left(80, 450);          // Slight left turn to knock tower
    msleep(200);
    
    backward(80, 650);      // Back up
    msleep(200);
    
    forward(90, 950);       // Final ram to square up
    
    // Final park position
    dozerup();
    stop();
    disable_servos();
    
    return 0;
}
