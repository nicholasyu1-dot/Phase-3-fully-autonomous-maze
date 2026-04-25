
#define PWR_R 5               // Right Motor Power
#define PWR_L 6               // Left Motor Power
#define MTR_R 8               // Right Motor Control
#define MTR_L 7  
#define MTR_ENABLE 3          // Motor Enable Pin
#include <Servo.h>


#define SERVO 10 
Servo scanServo;              // Servo



void forward(int SPEED){
    analogWrite(PWR_R,SPEED);
    analogWrite(PWR_L,SPEED);
    digitalWrite(MTR_R,HIGH);
    digitalWrite(MTR_L,HIGH);}

void backward(int SPEED){
    analogWrite(PWR_R,SPEED);
    analogWrite(PWR_L,SPEED);
    digitalWrite(MTR_R,LOW);
    digitalWrite(MTR_L,LOW);}

void stop(){
  analogWrite(PWR_R,0);
  analogWrite(PWR_L,0);}



void right(){
  digitalWrite(MTR_R,LOW);
  digitalWrite(MTR_L,HIGH);

    }

void left(){
    
    digitalWrite(MTR_R,HIGH);
    digitalWrite(MTR_L,LOW);

  }

void slow(){
    analogWrite(PWR_R,45);
    analogWrite(PWR_L,(45));


  }


void setServoAngle(int angle) {
  static int lastAngle = -1;
  angle = constrain(angle, 0, 180);

  if (angle != lastAngle) {
    scanServo.write(angle);
    delay(15);  // Allow servo to settle
    
    lastAngle = angle;

  }
}


// Center the servo
void centerServo() {
  setServoAngle(95);
}


void setspeed(int SPEED){
    analogWrite(PWR_R,SPEED);
    analogWrite(PWR_L,(SPEED));

}