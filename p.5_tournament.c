#include <kipr/wombat.h>

//positive speed value for forward (towards claw) and negative speed value for backwards (towards pusher)
void f_b(int speed, int time){
    motor(0, speed);
    motor(3,speed);
    msleep(time);
}

void left(int speed, int time){
    motor(0,speed);
    msleep(time);
}

void right(int speed, int time){
    motor(3,speed);
    msleep(time);
}

//motor(port, power);  |  Use da motor
//msleep(milliseconds);  |  time the motor/servo has to do their action
//enable_servo(port);  |  Enable the servo
//disable_servo(port);  |  Disable the servo
//set_servo_position(port, position);  |  Set position of a servo
//ao();  |  Stop all motors


int main()
{
	left(200,1000);
    f_b(-200,500);
    left(200,250);
    f_b(-200,100);
    left(200,350);
    f_b(-200,100);
    left(200,75);
    
    ao();
    
    //pushpoms
    f_b(-350,5500);
    //comeback
    f_b(350,5500);
    ao(); 
    //rest orientation
    f_b(200,100);
    left(-200,350);
    f_b(200,100);
    left(-200,250);
    f_b(200,500);
    //left(-200,1000);
    ao(); 
    return 0;
}
