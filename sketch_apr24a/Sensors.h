#include "Motors.h"
#include <Vector.h>

#define US_OUT 13             // Ultrasonic Sensor Input
#define US_IN 12              // Ultrasonic Sensor Output
#define LINE_L A2             // Left Line Tracker
#define LINE_C A1             // Center Line Tracker
#define LINE_R A0   
#define GYRO 0x68   
int16_t gyroZ;                // Raw gyro Z-axis reading
float gyroZOffset = 0;        // Calibration offset
float currentAngle = 0;       // Current angle in degrees
unsigned long lastTime = 0;   


// ====== GYRO FUNCTIONS ======

// Initialize Gyro Sensor


bool setupGyro() {


  Wire.begin();
  Wire.beginTransmission(GYRO);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // Wake up MPU6050
  byte error = Wire.endTransmission();
  
  if (error != 0) {
    return false;
  }
  
  // Configure gyro sensitivity (±250 deg/s)
  Wire.beginTransmission(GYRO);
  Wire.write(0x1B);  // GYRO_CONFIG register
  Wire.write(0x00);  // ±250 deg/s
  Wire.endTransmission();
  
  lastTime = micros();
  return true;
}

// Calibrate gyro (robot must be stationary!)
void calibrateGyro() {
  delay(500);
  
  long sum = 0;
  int samples = 100;
  
  for (int i = 0; i < samples; i++) {
    Wire.beginTransmission(GYRO);
    Wire.write(0x47);  // GYRO_ZOUT_H register
    Wire.endTransmission(false);
    Wire.requestFrom(GYRO, 2, true);
    
    int16_t gz = Wire.read() << 8 | Wire.read();
    
    sum += gz;
    delay(10);
  }
  
  gyroZOffset = sum / samples;
  currentAngle = 0;
}

// Read gyro Z-axis
int16_t readGyroZ() {
  Wire.beginTransmission(GYRO);
  Wire.write(0x47);  // GYRO_ZOUT_H register
  Wire.endTransmission(false);
  Wire.requestFrom(GYRO, 2, true);
  
  int16_t gz = Wire.read() << 8 | Wire.read();
  return gz;
}

// MUST be called frequently (e.g., every loop iteration)
// Angle accuracy degrades if this is not called often
void updateGyroAngle() {

  
  unsigned long now = micros();
  float dt = (now - lastTime) / 1000000.0;  // Time in seconds
  lastTime = now;
  
  if (dt <= 0 || dt > 0.1) {
    lastTime = micros();
  }
  // Read gyro
  gyroZ = readGyroZ();
  
  // Convert to degrees per second (sensitivity = 131 for ±250 deg/s)
  // INVERTED THE SIGN HERE to fix direction!
  float gyroRate = -((gyroZ - gyroZOffset) / 131.0);

  currentAngle += gyroRate * dt;
  
  // Keep angle in range -180 to +180
  if (currentAngle > 180) currentAngle -= 360;
  if (currentAngle < -180) currentAngle += 360;


}

// Reset angle to zero
void resetAngle() {
  currentAngle = 0;
}

// Get current angle
float getAngle() {
  return currentAngle;
}

// ===== ULTRASONIC SENSOR FUNCTIONS =====

// Returns distance in centimeters, or 0 if invalid
int getDistance() {
  int validReading = 0;
  int attempts = 0;
  
  while (validReading == 0 && attempts < 3) {
    if (attempts > 0) delay(60);  // Only delay on retries
    
    digitalWrite(US_OUT, LOW);
    delayMicroseconds(2);
    digitalWrite(US_OUT, HIGH);
    delayMicroseconds(10);
    digitalWrite(US_OUT, LOW);
    
    long duration = pulseIn(US_IN, HIGH, 30000);
    int distance = duration * 0.034 / 2;
    
    if (duration > 0 && distance <= 200) {
      validReading = distance;
    }
    
    attempts++;
  }


  return validReading;
}


//turns the robot to said angle, angle, left/rightm and speed can be set, left would be 0, and right woudl be 1
void turn(int angle, int side,int speed = 45){
  stop();


  delay(600);
  resetAngle();
  
  //will turn until it reaches that degree.
  while (not(abs(round((getAngle())))>(angle-0.05) && abs(round((getAngle())))<(angle+0.05))){
      setspeed(speed);
      updateGyroAngle();

  
  if (side == 1){
    right();

     }

  if (side == 0){
    
    left();

  }

  }

    resetAngle();
    delay(100);
  

}


// adjusts the angle to correct for drift
void adjustangleTo(float targetAngle,int speed) {
    const int minPower = 30;
    const int maxPower = 100;
    const float deadband = 1.0;

    while (true) {
        updateGyroAngle();

        float angleError = targetAngle - getAngle();
        if (angleError > 180) angleError -= 360;
        if (angleError < -180) angleError += 360;

        if (abs(angleError) <= deadband) break;  // aligned

        int power = constrain(abs(angleError) * 5, minPower, maxPower);

        if (angleError > 0) { // clockwise
            setspeed(power);

            left();
        } else {              // counter-clockwise
            setspeed(power);
            right();
        }
    }

    stop();
    digitalWrite(MTR_R, LOW);
    digitalWrite(MTR_L, LOW);}

//finds the best angle to turn by detecting the distances.
int find_optimal_wall_angle(Vector<int> distances){//returns index of distance 
    int count = 0;
    int index = 0;
    int largest_distance = 0;
    int avg_distance = 0;
    int sum_distance = 0;

    for (const int& distance : distances) { 
      count +=1;

      sum_distance +=distance;

      if (largest_distance < distance){
        index = count;
        largest_distance = distance;

      }
    }
    avg_distance = sum_distance/count;

    if (avg_distance !=0 && avg_distance <7){return -1;}

    
    
    
    return index;



}

//this code will turn  based on which mode its set to and only turns if it detects wall or line, depending on the mode, can be forecd to s aside of 1 being right and 2 being right, 0 being neither
int wall_or_line_turn(int mode, int forcedSide){



    updateGyroAngle();
    int angle = getAngle();


    if (mode == 0){
    if (analogRead(LINE_L) < 500 ){

    turn(90,0);
    }


    else if (analogRead(LINE_R)< 500 ){

    turn(90,1);
    }
    }

    
    else if (mode == 1){
    const int ELEMENT_COUNT_MAX = 50;
    int storage_array[ELEMENT_COUNT_MAX];
    Vector<int> angles(storage_array);
    Vector<int> distances(storage_array);
    
      angles.clear();
      distances.clear();


      int Serv_angle =95;
      setServoAngle(Serv_angle);

      int distance = getDistance();
      if (distance <=10 && distance != 0){


      stop();
      delay(100);

      
      
      
        for (int i = 0; i<18; i++){
          Serv_angle -=10;
          distance = getDistance();

          angles.push_back(Serv_angle);

          distances.push_back(distance);
          delay(50);
          setServoAngle(Serv_angle);


          }
      

        for (int i = 0; i<32; i++){
          Serv_angle +=10;
          distance = getDistance();

          angles.push_back(Serv_angle);

          distances.push_back(distance);
          delay(50);
          setServoAngle(Serv_angle);
          }

        

    
    setServoAngle(Serv_angle);
    }


    if (distances.size() ==0 ){return 0;}
    int index = find_optimal_wall_angle(distances);
    if (index == -1){

      turn(180,0);
      return 3;
    }
    else{
    int optimal_angle = 0;


    int count_idx = 0;
    for (const int& angle : angles) { 
        count_idx+=1;
        optimal_angle = angle;

        if (count_idx == index){break;}
    }


    setServoAngle(optimal_angle);
    if (forcedSide !=0){
    turn((90),forcedSide);
    return forcedSide;}
    if (optimal_angle>95){
    turn((90),1);       return 1;}

    if (optimal_angle<95){
    turn((90),0);      return 2;}

  } return 0;}


}

//this code will stop the robot, then,a fter 1 second, go forward, speed can be changed
void stop_and_go (int SPEED){
  stop();
  delay(1000);
  forward(SPEED);
  
}


//older code for drift correction, simple.
void adjustangle(){
  
  setspeed(51);
  while (not((getAngle())>(-1) && (getAngle())<(1))){
  updateGyroAngle();

  if (round(getAngle()) > 0){
      


      right();}

  else if  (round(getAngle()) < 0){

      left();}


}
}
//simple but quite effective linetracking code, will track if called constantly, and colour required.
void linetracking(int colour){


  if (analogRead(LINE_L)>colour){
    left();
    
  }
 if (analogRead(LINE_R)>colour){
    right();

  }

  if (analogRead(LINE_C)<colour){   
      forward(60);


  }



}