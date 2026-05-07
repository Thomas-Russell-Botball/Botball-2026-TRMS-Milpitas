#include <kipr/wombat.h>
#define BLACK_VALUE 1300
#define LMOTOR 0
#define RMOTOR 3
#define LEFT_TOPHAT 3
#define RIGHT_TOPHAT 2

#define LEFT_THRESHOLD 330
#define RIGHT_THRESHOLD 3680
#define BASE_SPEED 2000
#define TURN_FACTOR 1
#define LEFT_TURN_FACTOR 0.7

void forward(int speed, int time) {
    motor(0, speed);
    motor(3, speed);
    msleep(time);
}

void backward(int speed, int time) {
    motor(0, -speed);
    motor(3, -speed);
    msleep(time);
}

void right(int speed, int time) {
    motor(0, speed);
    motor(3, -speed);
    msleep(time);
}

void left(int speed, int time) {
    motor(0, -speed);
    motor(3, speed);
    msleep(time);
}



void line_follow(int duration_ms){
    int elapsed = 0; 

    while(elapsed < duration_ms){
        int left = analog(LEFT_TOPHAT);
        int right = analog(RIGHT_TOPHAT);

        if(left < LEFT_THRESHOLD && right < RIGHT_THRESHOLD){  // both black
            mav(0, -BASE_SPEED);
            mav(3, -BASE_SPEED);
        }
        else if(left < LEFT_THRESHOLD){  // left black= turn left
            mav(0, -BASE_SPEED * TURN_FACTOR);
            mav(3, -BASE_SPEED);
        }
        else if(right < RIGHT_THRESHOLD){ // right black= turn right
            mav(0, -BASE_SPEED);
            mav(3, -BASE_SPEED * TURN_FACTOR);
        }
        else{ // lost line= move slowly forward
            mav(0, -BASE_SPEED * 0.7);
            mav(3, -BASE_SPEED * 0.7);
        }

        msleep(10);
        elapsed += 10;
    }

    // Stop motors
    mav(0, 0);
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
    line_follow(5500);
    forward(50,6500);
    right(100,400);
    backward(100,1000);
    left(100,450);
    backward(100,2350);
    forward(100,6000);


    disable_servos();



    return 0;
}
