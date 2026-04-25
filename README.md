# Autonous maze
This repository is for my Code base and, readme file for this assignment, using the code I made for the Autonous ,included.
This Progam will be able to navigate a simple course assumeing the walls are made of , well an object also MAKE SURE THE CORENERS ARE 90DEG AS THIS IS JUST HOW ITS WRITTEN.
## How it works
this program has 2 modes, maze solving, and linetracking.
When it detects a black line in line tracking mode, it will swithc back to maze mode
It switcehd to line tracking by going over a white line. 
Its defualt is linetracking.
after wswitching back to maze solving the 2nd time, so at the 2nd black line, it will do a celebrating, in which it wont move and u just need to press the reset button

this  program is also able to track a line too, and, with that, you could link it to a 2nd maze, where it switches modes back
I'll be able to turn 180 DEG if its sees 2 walls on 3 sides.
I'll be able to notice when it has gone in a loop.
This code also has colour indicatros for modes,
Green for maze solving
white for line tracking
and purple for celebrating
the celebrating will consist of  move forwards, then moves backwardss a bit 4 times, then does one a 180, and loops this 4 times total before stoping the motors. 
there codes split up, so if you want u can make your own programs too.

## How to run
To run this you will first need the ELEGOO SMART ROBOT CAR KIT 4.0, not sure if the others will work. 
Next, you will need the arduino IDE and put the .ino into a .ino file or dowload the .ino file along with all the .h files
Then, you will need to plug in the robot using the usb-A to the robot upload port, and MAKE SURE ITS ON UPLOAD MODE
Next, you need to select the Arduino uno board in the top left corner and select the port for the robot, mostlikely will only be one.
Then, press the upload button looks like this (->) so that the code will be put on the robot.

Now that the code is uploaded onto the robot simply unplug it, place it down in a maze
and turn it on.
then, press the black long button that is on the same side ans the upload port.
then it should run and go through the maze

# THINGS OF NOTE
This code is written and calibarted for MY ROBOT
So, for example: the Servo motor is supposed to be centered when set to 90, but mine is centered at 95 so change the code to your robot
Also: i think  my left motors are slower so that it will drift a bit to that side, but there is code for correction
DRIFT IS NORMAL FOR THIS BOARD.
The line tracking is also per-robot so the 325 is white masking tape for my robot but may be differnt for your robots
the line tracking using white is at 290 or less for white and 980 or more for black.
