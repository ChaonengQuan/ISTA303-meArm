/* 
  Assignment 3 (MeArm) Scaffold
  Kinematics code for MeArm
  Do not use or look at the open code available for the MeArm -- you must write your own!
  NOTE: Must change "NO LINE ENDING" to "NEWLINE" in Serial Monitor (button on top right) for this to work. 
*/

#include <Servo.h>

// Servo pins
#define CLAW_PIN  6
#define RIGHT_PIN  9
#define LEFT_PIN  10
#define MIDDLE_PIN  11

// Create servo objects to control servos
Servo CLAW;  
Servo RIGHT;  
Servo LEFT;  
Servo MIDDLE;  

// Global variables for storing the position to move to
int moveToTheta     = 0;
int moveToR         = 0;
int moveToZ         = 0;
int moveToGripper   = 0;

/*
 * Functions (
 */

// Move the rotational (theta, polar coordinates) axis of the MeArm
void moveTheta(int angle) {
  MIDDLE.write(angle);
}

// Open the gripper a given width/distance
void moveGripper(int distToOpen) {
  distToOpen -= 5;
  int thetaX = (180*distToOpen)/(76+distToOpen);
  int servo = (100-thetaX)+10;
  CLAW.write(servo);
}

// Move the arm along the r axis (polar coordinates), or in height (z)
void moveRZ(int r, int z) {
  moveLeft(getAngleW(r,z));
  moveRight(getAngleBK(r,z));
}

float getAngleC(int r, int z){
  int c = sqrt(r*r + z*z);
  int angleC = (c * 180) / (162 + c);
  return angleC;
}

float getAngleW(int r, int z){
  float C = getAngleC(r,z);
  float W = -C/2 - atan(r/z)*(2*PI/360) + 90; 
  Serial.print("angle W is :        ");
  Serial.println(W);
  return W;
}

float getAngleBK(int r, int z){
  Serial.print("angle BK is :        ");
  Serial.println(180 - getAngleC(r,z) - getAngleW(r,z));
  return 180 - getAngleC(r,z) - getAngleW(r,z);
}

/*
 * Arduino core (setup, loop)
 */
 
void setup() {
  // Enable serial port output for debug
  Serial.begin(9600);
  Serial.println("MeArm Initializing...");
 
  // Attaches the servo objects to servos on specific pins
  CLAW.attach(CLAW_PIN);
  RIGHT.attach(RIGHT_PIN);
  LEFT.attach(LEFT_PIN);
  MIDDLE.attach(MIDDLE_PIN);
  
}

// Display a simple serial console to the user that allows them to enter positional information for the MeArm to move to. 
void doSerialConsole() {
  // Display serial console 
  char inputStr[80];
  int index = 0;
  
  while (1) {
    int done = 0;
    // Step 1: Display serial console message
    Serial.println("");
    Serial.println("Enter coordinates: theta,r,z,gripper (comma delimited, no spaces)");
    Serial.println("Example: 10,20,30,40");

    // Step 2: Clear string
    for (int i=0; i<80; i++) {
      inputStr[i] = 0;
    }
    index = 0;

    // Step 3: Read serial data
    while (done == 0) {    
      // Step 3A: Read serial data and add to input string, if available
      while(Serial.available() > 0) {
        char inputChar = Serial.read();        
        if (inputChar == '\n') {          
          // Newline character -- user has pressed enter
          done = 1;                
        } else {
          // Regular character -- add to string
          if (index < 79) {
            inputStr[index] = inputChar;                  
            index += 1;
          }
        }      
      }      
    }

    // Step 4: Debug output: Let the user know what they've input
    Serial.print ("Recieved input: ");
    Serial.println(inputStr);

    // Step 5: Check if string is valid
    if (sscanf(inputStr, "%d,%d,%d,%d", &moveToTheta, &moveToR, &moveToZ, &moveToGripper) == 4) {  
      Serial.println("Valid input!");  
      // Valid string
      return;
    } else {
      // Invalid string -- restart
      Serial.println ("Invalid input -- try again.  example: 10,20,30,40");      
    }      
  }    
}

void moveLeft(int angle) {
    int offset = (angle - 45) * 2;
    LEFT.write(angle - offset);
}

void moveRight(int angle) {
    int offset = (angle - 90) * 2;
    RIGHT.write(angle - offset);
}

void loop() {
  
//  // Step 1: Display Serial console
//  doSerialConsole();
//
//  // Step 2: Debug display
//  Serial.println("");
//  Serial.println("Moving to: ");
//  Serial.print("Theta: ");
//  Serial.println(moveToTheta);
//  Serial.print("R: ");
//  Serial.println(moveToR);
//  Serial.print("Z: ");
//  Serial.println(moveToZ);
//  Serial.print("Gripper: ");
//  Serial.println(moveToGripper);
//  
//
//  // Step 3: Move to requested location
//  moveTheta(moveToTheta);
//  moveRZ(moveToR, moveToZ);
//  moveGripper(moveToGripper);  
//    LEFT.write(30);
//    RIGHT.write(135);
//    moveLeft(0);
//    moveRight(90);
//    moveGripper(70);
//    //delay(1000);
//    //CLAW.write(110);
//    //delay(5000);
//    //CLAW.write(180);
//    //delay(400);
////    Serial.print("Angle C: ");
////    Serial.println(getAngleC(10,10));
//    Serial.print("Angle W: ");
//    Serial.println(getAngleW(10,10));
  
 
}
