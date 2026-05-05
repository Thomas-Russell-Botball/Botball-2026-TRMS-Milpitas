#include <kipr/wombat.h>

// Motor ports (per provided mapping)
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
    motor(0, speed);
    motor(1, -speed); 
    motor(2, speed); 
    motor(3, -speed);
    msleep(time);
}

void backward(int speed, int time) {
    motor(0, -speed);
    motor(1, speed);
    motor(2, -speed);
    motor(3, speed);
    msleep(time);
}

void right(int speed, int time) {
    motor(0, -speed);
    motor(1, speed);
    motor(2, speed);
    motor(3, -speed);
    msleep(time);
}

void left(int speed, int time) {
    motor(0, -speed);
    motor(1, speed);
    motor(2, speed);
    motor(3, -speed);
    msleep(time);
}

void dozer(int position, int time){
    set_servo_position(2,position);
    set_servo_position(3,position);
    msleep(time);
}

void dozerup(){
    enable_servos();
    dozer(2300, 1000);
    enable_servos();
}
    
void dozerdown(){
    dozer(975, 1000);
}


void line_follow(int duration_ms){
    int elapsed = 0; 

    while(elapsed < duration_ms){
        int left = analog(LINE_SENSOR);
        int right = analog(LINE_SENSOR);

        if(left < LEFT_THRESHOLD && right < RIGHT_THRESHOLD){  // both black
            mav(0, BASE_SPEED);
            mav(1, BASE_SPEED);
            mav(2, BASE_SPEED);
            mav(3, BASE_SPEED);
        }
        else if(left < LEFT_THRESHOLD){  // left black= turn left
            mav(0, BASE_SPEED * TURN_FACTOR);
            mav(1, BASE_SPEED * TURN_FACTOR);
            mav(2, -BASE_SPEED);
            mav(3, -BASE_SPEED);
        }
        else if(right < RIGHT_THRESHOLD){ // right black= turn right
            mav(0, -BASE_SPEED);
            mav(1, -BASE_SPEED);
            mav(2, BASE_SPEED * TURN_FACTOR);
            mav(3, BASE_SPEED * TURN_FACTOR);
        }
        else{ // lost line= move slowly forward
            mav(0, BASE_SPEED * 0.7);
            mav(1, BASE_SPEED * 0.7);
            mav(2, BASE_SPEED * 0.7);
            mav(3, BASE_SPEED * 0.7);
        }

        msleep(10);
        elapsed += 10;
    }

    // Stop motors
    mav(0, 0);
    mav(1, 0);
    mav(2, 0);
    mav(3, 0);
}

void freeeze(){
    mav(0, 0);
    mav(1, 0);
    mav(2, 0);
    mav(3, 0);
}


void servo(int port, int position, float time){
    float start_time = seconds();
    int start_pos = get_servo_position(port);
    int pos_diff = position - start_pos;
    while(seconds() - start_time < time){
        float pos_mod = (seconds()-start_time)/time;
        set_servo_position(port, start_pos + (pos_diff * pos_mod));
    }
    set_servo_position(port, position);
    msleep(50);
}


int main() {
    wait_for_light(5);
    shut_down_in(118);
    dozerup();
    
    //line_follow(5500);
    forward(50,1000); //5750
    msleep(200);
    right(100,600);
    
    forward(100,5000);
    
    right(100,800);
    
    dozerdown();
    
    forward(100,5000);
    
   
    //left(100,450);
    //backward(100,2350);
    //forward(100,6000);
    freeeze();
    

    disable_servos();
    


    return 0;
}
