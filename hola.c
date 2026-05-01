#include <stdio.h>
#include <kipr/wombat.h>
#include <stdlib.h>

#define LEFTMOTOR1 0 //#define RIGHT_BACK_MOTOR 0
#define LEFTMOTOR2 1 //#define RIGHT_FRONT_MOTOR 1
#define RIGHTMOTOR1 2 //#define LEFT_BACK_MOTOR 2
#define RIGHTMOTOR2 3 //#define LEFT_FRONT_MOTOR 3
#define LIGHTSENSOR1 0
#define LIGHTSENSOR2 1
#define compensation_rate 0.2
#define claw_click 0

    void ow_turnr_90() {
        mav(RIGHTMOTOR1, 1000);
        mav(RIGHTMOTOR2, 1000);
 		mav(LEFTMOTOR1, -1000);
        mav(LEFTMOTOR2, -1000);
        msleep(2500);
       	mav(RIGHTMOTOR1, 0);
        mav(RIGHTMOTOR2, 0);
  		mav(LEFTMOTOR1, 0);
       	mav(LEFTMOTOR2, 0);
        msleep(20);
    }
    
	void drive(int cm, int speed){
        float move = 2000*(cm/27);
  		cmpc(RIGHTMOTOR1);
  		while(abs(gmpc(RIGHTMOTOR1)) < move){
    		mav(RIGHTMOTOR1, -speed);
        	mav(RIGHTMOTOR2, speed);
    		mav(LEFTMOTOR1, speed);
        	mav(LEFTMOTOR2, speed);
  		}
  			mav(RIGHTMOTOR1, 0);
        	mav(RIGHTMOTOR2, 0);
  			mav(LEFTMOTOR1, 0);
        	mav(LEFTMOTOR2, 0);
  			msleep(20);
		}
    
    void turnright(float distance, int speed){
  		cmpc(LEFTMOTOR1);
  		while(fabs(gmpc(LEFTMOTOR1)) < 1700*distance/90.0){
    		mav(RIGHTMOTOR1, -speed);
            mav(RIGHTMOTOR2, speed);
    		mav(LEFTMOTOR1, speed);
            mav(LEFTMOTOR2, -speed);
  		}
  		mav(RIGHTMOTOR1, 0);
        mav(RIGHTMOTOR2, 0);
  		mav(LEFTMOTOR1, 0);
       	mav(LEFTMOTOR2, 0);
  		msleep(20);
    }
    void turnleft(float distance, int speed){
  		cmpc(RIGHTMOTOR2);
  		while(fabs(gmpc(RIGHTMOTOR2)) < 1700*distance/90.0){
    		mav(RIGHTMOTOR1, speed);
            mav(RIGHTMOTOR2, -speed);
    		mav(LEFTMOTOR1, -speed);
            mav(LEFTMOTOR2,  speed);
  		}
  		mav(RIGHTMOTOR1, 0);
        mav(RIGHTMOTOR2, 0);
  		mav(LEFTMOTOR1, 0);
       	mav(LEFTMOTOR2, 0);
  		msleep(20);
    }
    void linefollow(int distance, int speed, int port,float compensation_rate){
       cmpc(port);
        while (abs(gmpc(port)) < distance) {
        int error = analog(2) - (3000);
        float speed_modifier = error * compensation_rate;
        printf("%i\n",error);
        mav(LEFTMOTOR1, speed - speed_modifier);
        mav(LEFTMOTOR2, -speed + speed_modifier);
        
        mav(RIGHTMOTOR1, speed + speed_modifier);
        mav(RIGHTMOTOR2, -speed - speed_modifier);
        }
        freeze(0);
        freeze(1);
        freeze(2);
        freeze(3);
  
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
            /* printf("%i,",gmpc(0));
             printf("%i,",gmpc(1));
             printf("%i,",gmpc(2));
             printf("%i,",gmpc(3));
            printf("\n");
            printf("%f",position);
            printf("\n");*/

            
            
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
        //set_servo_position(0,780);//780 is Up 2047 is down
        //set_servo_position(1,0);
        set_servo_position(2,2300);
        set_servo_position(3,2300);

   msleep(200);
    //move forward 40 cm
    pidMove(70,1500,0.3);
    //turn left 90
    turnleft(90,1500);
    msleep(1000);
    
    //square up
    pidMove(-500,1500,485);
    turnright(1,400);
    //Forward 
    
    set_servo_position(2,1170);
    set_servo_position(3,1100);
    	msleep(1000);

        pidMove(150,1500,0.3);
    turnleft(2,1500);
     pidMove(50,1500,0.3);
	msleep(500);
    turnright(240,400);
    turnright(220,400);
    msleep(1000);
  
    pidMove(150,1500,0.3);


    
   



    disable_servos();
    return 0;

}
