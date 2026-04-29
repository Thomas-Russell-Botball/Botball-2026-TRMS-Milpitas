#include <kipr/wombat.h>
#include <stdlib.h>

// Motor ports (per provided mapping)
#define RIGHT_BACK_MOTOR 0
#define RIGHT_FRONT_MOTOR 1
#define LEFT_FRONT_MOTOR 2
#define LEFT_BACK_MOTOR 3

// Servo ports
#define SERVO_FRONT_RIGHT 2
#define SERVO_FRONT_LEFT 3

// Sensor ports
// Digital: start light sensor
#define START_LIGHT_SENSOR 3
// Analog: tophat sensors for squaring up and line follow
#define SQUARE_LEFT_SENSOR 2
#define LINE_SENSOR 1
#define SQUARE_RIGHT_SENSOR 0

#define LINE_TARGET 3000

void drive(int cm, int speed){
    float move = 2000*(cm/27);
    cmpc(LEFT_FRONT_MOTOR);
    while(abs(gmpc(LEFT_FRONT_MOTOR)) < move){
        mav(RIGHT_FRONT_MOTOR, -speed);
        mav(RIGHT_BACK_MOTOR, speed);
        mav(LEFT_FRONT_MOTOR, speed);
        mav(LEFT_BACK_MOTOR, speed);
    }
    mav(RIGHT_FRONT_MOTOR, 0);
    mav(RIGHT_BACK_MOTOR, 0);
    mav(LEFT_FRONT_MOTOR, 0);
    mav(LEFT_BACK_MOTOR, 0);
    msleep(20);
}

void turnright(float distance, int speed){
    cmpc(LEFT_BACK_MOTOR);
    while(fabs(gmpc(LEFT_BACK_MOTOR)) < 1700*distance/90.0){
        mav(RIGHT_FRONT_MOTOR, -speed);
        mav(RIGHT_BACK_MOTOR, speed);
        mav(LEFT_FRONT_MOTOR, speed);
        mav(LEFT_BACK_MOTOR, -speed);
    }
    mav(RIGHT_FRONT_MOTOR, 0);
    mav(RIGHT_BACK_MOTOR, 0);
    mav(LEFT_FRONT_MOTOR, 0);
    mav(LEFT_BACK_MOTOR, 0);
    msleep(20);
}

void turnleft(float distance, int speed){
    cmpc(LEFT_FRONT_MOTOR);
    while(fabs(gmpc(LEFT_FRONT_MOTOR)) < 1700*distance/90.0){
        mav(RIGHT_FRONT_MOTOR, speed);
        mav(RIGHT_BACK_MOTOR, -speed);
        mav(LEFT_FRONT_MOTOR, -speed);
        mav(LEFT_BACK_MOTOR,  speed);
    }
    mav(RIGHT_FRONT_MOTOR, 0);
    mav(RIGHT_BACK_MOTOR, 0);
    mav(LEFT_FRONT_MOTOR, 0);
    mav(LEFT_BACK_MOTOR, 0);
    msleep(20);
}

// Robust line follow (based on Rancho MMS pattern)
void linefollow(int distance, int speed, int encoder_port, float compensation_rate){
    cmpc(encoder_port);
    while (abs(gmpc(encoder_port)) < distance) {
        int error = analog(LINE_SENSOR) - LINE_TARGET;
        float speed_modifier = error * compensation_rate;

        mav(LEFT_FRONT_MOTOR, speed - speed_modifier);
        mav(LEFT_BACK_MOTOR, -speed + speed_modifier);

        mav(RIGHT_FRONT_MOTOR, speed + speed_modifier);
        mav(RIGHT_BACK_MOTOR, -speed - speed_modifier);
    }
    mav(RIGHT_FRONT_MOTOR, 0);
    mav(RIGHT_BACK_MOTOR, 0);
    mav(LEFT_FRONT_MOTOR, 0);
    mav(LEFT_BACK_MOTOR, 0);
    msleep(20);
}

void pidMove(float distance, float topSpeed,float finalError){
    cmpc(0); cmpc(1); cmpc(2); cmpc(3);
    float KP = 10.0;   //still need to be tuned
    float KI = 0.01;   //still need to be tuned
    float KD = 0.1;  //still need to be tuned

    //initialization
    float error = distance;
    float lastError = distance;
    float integral = 0;
    float dt =20;

    while(fabsf(error)>finalError){
        //1. Get input
        float position = (gmpc(0)-gmpc(1)+gmpc(2)-gmpc(3));
        //40 cm is 1500
        position/=4;
        position/=2;
        position/=37.5;

        //2. Calculate error
        error = distance - position;

        //3. Calculate terms
        integral = integral + (error * dt);
        if(integral/fabs(integral)!=error/fabs(error)){
            integral=0;
        }

        int derivative = (error - lastError) / dt;

        //4. Calculate output
        float output = (KP * error) + (KI * integral) - (KD * derivative);

        //Makes sure speed isn't over top speed
        if(fabs(output)>topSpeed){
            output/=fabs(output);
            output*= topSpeed;
        }

        //5. Apply output and save state
        mav(0,output);
        mav(1,-output);
        mav(2,output);
        mav(3,-output);
        lastError = error;

        //6. Wait for next time step
        msleep(dt);
    }
    freeze(0);
    freeze(1);
    freeze(2);
    freeze(3);
}

int main()
{
    enable_servos();
    set_servo_position(0,780);//780 is Up 2047 is down
    set_servo_position(1,0);
    set_servo_position(2,2300);
    set_servo_position(3,2300);

    msleep(200);
    //move forward 40 cm
    pidMove(57,1500,0.3);
    //turn left 90
    turnleft(100,1500);
    msleep(1000);

    //square up
    pidMove(-500,1500,490);
    //turnleft(8,400);

    //Forward

    set_servo_position(2,1170);
    set_servo_position(3,1100);

    msleep(2000);
    pidMove(30,1500,0.3);
    //turnright(2,1500);
    pidMove(160,1500,0.3);
    msleep(2000);

    mav(LEFT_FRONT_MOTOR, 1500);
    mav(LEFT_BACK_MOTOR, -1500);
    mav(RIGHT_FRONT_MOTOR, 100);
    mav(RIGHT_BACK_MOTOR, -100);
    msleep(5150);
    mav(LEFT_FRONT_MOTOR, 0);
    mav(LEFT_BACK_MOTOR, 0);

    pidMove(155,1500,0.5);
    turnright(90,1000);
    pidMove(20,1500,0.5);
    turnright(45,1000);
    pidMove(80,1500,0.5);

    pidMove(-30,1500,0.5);

    disable_servos();
    return 0;
}
