#include <EvoLib.h>
#include <AccelStepper.h>

#include <avr/wdt.h>
#include <stdio.h>
#include <ctype.h>

//SOFTWARE END LIMITS
#define XLIMITFAR 687
#define YLIMITFAR 581
#define XLIMITNEAR 65
#define YLIMITNEAR 65
#define TESTHOMEXBOUNCE 35
#define TESTHOMEYBOUNCE 10

//EXTUDER SETUP
#define PAUSETIME 400
EvoExtruder extruder(PAUSETIME,4,5,6,7);

//MOTOR SETUP
#define STEPS 200
AccelStepper xStepper(AccelStepper::FULL4WIRE,53,51,49,47);
AccelStepper yStepper(AccelStepper::FULL4WIRE,52,50,48,46);
int xSpeed = 0;
int ySpeed = 0;

//ENDSTOP SETUP
EvoEndstop x1(31);
EvoEndstop x2(33);
EvoEndstop y1(35);
EvoEndstop y2(37);

//PARSING GLOBAL VARIABLES
String input = "";

void setup() {

  //SERIAL SETUP
  Serial.begin(9600);

  //MOTOR SETUP
  xStepper.setMaxSpeed(1000.0);
  yStepper.setMaxSpeed(1000.0);
  xStepper.setAcceleration(25.0);
  yStepper.setAcceleration(25.0);
  xStepper.setSpeed(xSpeed);
  yStepper.setSpeed(ySpeed);

  //Serial.println("S");
}

void loop() {
  //Read
  readSerial();

  //Eval
  input = parse(input);

  //Execute
  extruder.check();
  checkEndstopsAndAdjust();
  moveMotors();
}

//If any of the endstops are pressed, multiply the velocity of the given axis by -1 (reversing it at full speed)
void checkEndstopsAndAdjust() {
  if(x1.pressed() || x2.pressed()){
    //Serial.println("--XX--");
    xSpeed = xSpeed * -1;
    xStepper.setSpeed(xSpeed);
  }
  if (y1.pressed() || y2.pressed()) {
    //Serial.println("--YY--");
    ySpeed = ySpeed * -1;
    yStepper.setSpeed(ySpeed);
  }
}

//parses out a given command and updates the needed values
//returns true iff a valid command has been executed
String parse(String inputStr) {
  if(input.length() == 8 && sanitizeMotorInstruction()){
      //Serial.println(inputStr);
      String xStr = inputStr;
      String yStr = inputStr;
      xStr.remove(4,4);
      yStr.remove(0,4);
      int xSpd = xStr.toInt();
      int ySpd = yStr.toInt();
      updateMotorSpeeds(xSpd,ySpd);
      return "";
  } else if(input.length() == 8) {
    //This case should not occur.
    //Only returns true for testing so that it will reset on incorrect motor instruction inputs.
    return "";
  } else if(input == "e"){ //activate extuder
    extruder.extrude();
    return "";
  } else if(input == "p"){ //pause extruder
    extruder.pause();
    return "";
  } else if(input == "d") {
    disablePrinter();
    return "";
  } else if(input == "h") {
    homePrinter();
    return "";
  } else if(input == "t"){
    goToEval(true);
    return "";
  } else {
    return inputStr;
  }
}

//reads a bit from serial and adds it to the input string
void readSerial() {
  if(Serial.available() > 0){
    input += (char)Serial.read();
  }
}

//*********** MOTOR FUNCTIONS ************

//Returns true iff the instruction is in the form '+100+100'
bool sanitizeMotorInstruction() {
  if (input.length() == 8){
    char arr[9];
    input.toCharArray(arr,9);
    if((arr[0] == '+' || arr[0] == '-') &&
        isDigit(arr[1]) && isDigit(arr[2]) && isDigit(arr[3]) &&
          (arr[4] == '+' || arr[4] == '-') &&
            isDigit(arr[5]) && isDigit(arr[6]) && isDigit(arr[7]))
        {
          return true;
        }
    return false;
  }
  return false;
}

//Updates global speed variables and stepper speeds.
void updateMotorSpeeds(int xSpd, int ySpd) {
   updateXSpeed(xSpd);
   updateYSpeed(ySpd);
}
void updateXSpeed(int xSpd){
  xSpeed = xSpd;
  xStepper.setSpeed(xSpeed);
}
void updateYSpeed(int ySpd){
   ySpeed = ySpd;
   yStepper.setSpeed(ySpeed);
}

//Steps each motor one step iff it should.
void moveMotors() {
  if(xStepper.currentPosition() < XLIMITFAR && xStepper.speed() > 0){
    xStepper.runSpeed();
  } else if(xStepper.currentPosition() > XLIMITNEAR && xStepper.speed() < 0){
    xStepper.runSpeed();
  }

  if(yStepper.currentPosition() < YLIMITFAR && yStepper.speed() > 0){
    yStepper.runSpeed();
  } else if(yStepper.currentPosition() > YLIMITNEAR && yStepper.speed() < 0){
    yStepper.runSpeed();
  }
}

//Resets printer to default state, unlocks motors awaiting next command.
void disablePrinter() {
  xStepper.disableOutputs();
  yStepper.disableOutputs();
  extruder.pause();
  updateMotorSpeeds(0,0);
  xStepper.enableOutputs();
  yStepper.enableOutputs();
}

//"Homes" Printer to the center of the workable space.
void homePrinter() {
  //Send printer to bottom right hand corner
  goToEval(false);

  //move printer to center of software limits
  yStepper.moveTo(((YLIMITFAR - YLIMITNEAR) /2) + YLIMITNEAR);
  xStepper.moveTo(((XLIMITFAR - XLIMITNEAR) /2) + XLIMITNEAR);
  while(xStepper.distanceToGo() != 0 || yStepper.distanceToGo() != 0){
      if(xStepper.distanceToGo() != 0){
        xStepper.run();
      }
      if(yStepper.distanceToGo() != 0){
        yStepper.run();
      }
  }

  //DISABLE PRINTER AND SEND RESPONSE TO HOST
  disablePrinter();
  Serial.print('h');
}

void goToEval(bool confirm){
//HOME X AXIS
  updateMotorSpeeds(-100,-100);


  while(!x1.pressed()){
    xStepper.runSpeed();
  }
  xStepper.move(TESTHOMEXBOUNCE);
  while(xStepper.distanceToGo() != 0){
    xStepper.run();
  }


  while(!y1.pressed()){
    yStepper.runSpeed();
  }
    yStepper.move(TESTHOMEYBOUNCE);
  while(yStepper.distanceToGo() != 0){
    yStepper.run();
  }

  updateMotorSpeeds(0,0);

    //DISABLE PRINTER AND SEND RESPONSE TO HOST
  xStepper.setCurrentPosition(0);
  yStepper.setCurrentPosition(0);
  disablePrinter();
  if(confirm){
      Serial.print('t');
  }
}
