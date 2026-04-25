#include "Setup.h"

#include <Vector.h>
//constants
const int RECV_PIN = 11;

int SPEED = 100;
int colour = 290;
int Serv_angle = 95;
int last_turn = 0;
int time = 1;
int black = 980;

int times_switched_to_line_ctracker = 0;
int times_switched_to_maze = 0;
int mode = 0;  //0 is maze, 1 in line_tracker
unsigned long StartTime = millis();


const int ELEMENT_COUNT_MAX = 150;
int storage_array[ELEMENT_COUNT_MAX];
Vector<int> turn_type(storage_array);
Vector<int> time_taken(storage_array);
int forcedSide = 0;

//chekcs how much time has passes between its last call.
long get_time_passesd() {
  unsigned long CurrentTime = millis();
  unsigned long ElapsedTime = CurrentTime - StartTime;
  StartTime = millis();

  return ElapsedTime;
}

void setup() {
  start();
  ledOn(CRGB::Green);

}

void loop() {
  //mode switches,and celbration call
  if (analogRead(LINE_C) > black && mode == 0) {
    turn(180,0);

  }

  if (analogRead(LINE_C) < colour && mode != 1) {
    ledOn(CRGB::White);

    mode = 1;

    times_switched_to_line_ctracker += 1;
  }

  if (analogRead(LINE_C) > black && mode != 0) {
    mode = 0;
    ledOn(CRGB::Green);

    times_switched_to_maze += 1;
  }
  if (times_switched_to_maze >1){celeration();}

  //cheking for getting stuck in loops
  if (mode == 0) {
    int turned = wall_or_line_turn(1,forcedSide);
    if (turned == 1 || turned == 2 ) {
      unsigned long time_passed = get_time_passesd();
      turn_type.push_back(turned);
      time_taken.push_back(time_passed);
      if (forcedSide != 0){

        forcedSide = 0;
      }
    }


    int count = 0;
    int Turns_in_a_row = 0;
    int prev_turn = 0;
    for (const int& turn : turn_type) {


      if (count == 0) {
        prev_turn = turn;

      } else if (prev_turn == turn) {
        Turns_in_a_row += 1;
      }
      if (Turns_in_a_row == 4) { break; }
      count += 1;
      int count = 0;
    }

    int similar_times_in_a_row = 0;
    int prev_time = 0;
    if (similar_times_in_a_row == 4) {
      for (const int& time : time_taken) {

        if (count == 0) {

          prev_time = round(time);


        } else if (prev_time == round(time)) {
          similar_times_in_a_row += 1;
        }
        if (similar_times_in_a_row == 2) { 
          time_taken.clear();
          turn_type.clear();
          if (prev_turn == 0){
          forcedSide = 1;}
          else{ forcedSide = 0;}

          break; }
        count += 1;
        

      }
    }

    forward(SPEED);
  }
  if (mode == 1) {
    linetracking(colour);
  }
}
//celebration, will move forwards, then moves backwardss a bit 4 times, then does one a 180, and loops this 4 times total before stoping the motors. 
void celeration(){
  ledOn(CRGB::Purple);
  stop();
  delay(500);
  forward(120);
  delay(240);
  stop();
  delay(250);
  setspeed(100);
  for (int i =0; i<4;i++){

    for (int j =0; j<4;j++){
    backward(100);
    delay(500);
    stop();
    delay(500);

  ;}

  turn(180,0,100);}


  digitalWrite(MTR_ENABLE, LOW);

}
