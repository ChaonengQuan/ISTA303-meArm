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

// constants
const int ARMLEN = 81; // arm is 81mm

/*
 * Functions 
 */

// Move the rotational (theta, polar coordinates) axis of the MeArm
void moveTheta(int angle) {
  MIDDLE.write(angle);
}

// Open the gripper a given width/distance
// TODO: make this accurate
void moveGripper(int distToOpen) {
  distToOpen -= 5;
  int thetaX = (180*distToOpen)/(76+distToOpen);
  int servo = (100-thetaX)+10;
  CLAW.write(servo);
}

/*
 * I added the compressed method below according to my written math steps, not tested.
 * Chaoneng Quan
 */

//void moveRZ(int r, int z) {
//  int a = 81;
//  int b = 81;
//  float lengthOfc = sqrt(sq(r) + sq(z));
//  float angleOfC = acos( (sq(a) + sq(b) - sq(c))/2*a*b ) * (2*PI/360);
//  float angleOfB = (180 - angleOfC)/2;
//  float angleOfK = atan(z/r) * (2*PI/360)
//  float angleOfW = 180 - angleOfC - (angleOfB + angleOfK);
//
//  moveLeft(angleOfW);
//  moveRight(angleOfB + angleOfK);
//}


// Move the arm along the r axis (polar coordinates), or in height (z)
void moveRZ(int r, int z) {
  // calculate stuff in proper order
  float sideC = getSideC(r, z);
  float angleK = getAngleK(r, z);
  float angleB = getAngleB(sideC);
  float angleC = getAngleC(angleB);
  float angleW = getAngleW(angleC, angleB, angleK);

  moveLeft(angleW);
  moveRight(angleB + angleK);
}

/*
 * Triangle calculations
 * Refer to kinematics diagram to understand labels
 */

float getAngleC(float angleB){
  float angleC = 180 - 2 * angleB; // angleA == angleB
  //Serial.print("angle C is :        ");
  //Serial.println(angleC);
  return angleC;
}

float getAngleW(float angleC, float angleB, float angleK){
  float angleW = 180-angleC-angleB-angleK; 
  //Serial.print("angle W is :        ");
  //Serial.println(angleW);
  return angleW;
}

float getAngleB(float sideC){
  // divide triangle in half to get two (identical) right triangles
  float angleB = acos((sideC/2)/ARMLEN)*(2*PI/360);
  //Serial.print("angle B is :        ");
  //Serial.println(angleB);
  return angleB;
}

float getSideC (int r, int z){
  float sideC = sqrt(sq(r) + sq(z));
  //Serial.print("side c is :        ");
  //Serial.println(sideC);
  return sideC;
}

float getAngleK (int r, int z){
  float angleK = atan(r/z)*(2*PI/360);
  //Serial.print("angle K is :        ");
  //Serial.println(angleK);
  return angleK;
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
  
  // // Step 1: Display Serial console
  // doSerialConsole();

  // // Step 2: Debug display
  // Serial.println("");
  // Serial.println("Moving to: ");
  // Serial.print("Theta: ");
  // Serial.println(moveToTheta);
  // Serial.print("R: ");
  // Serial.println(moveToR);
  // Serial.print("Z: ");
  // Serial.println(moveToZ);
  // Serial.print("Gripper: ");
  // Serial.println(moveToGripper);
  // 

  // // Step 3: Move to requested location
  // moveTheta(moveToTheta);
  // moveRZ(moveToR, moveToZ);
  // moveGripper(moveToGripper);  

}
