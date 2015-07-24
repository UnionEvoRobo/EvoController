#include <EvoLib.h>
#include <AccelStepper.h>

#include <stdio.h>
#include <ctype.h>


#define STEPS 200

EvoExtruder extruder(300,4,5,6,7);
//EvoMotor xMotor(40,41,42,43);
//EvoMotor yMotor(51,52,53,54);

//MOTOR SETUP
AccelStepper xStepper(AccelStepper::FULL4WIRE,53,51,49,47);
AccelStepper yStepper(AccelStepper::FULL4WIRE,52,50,48,46);
int xspeed = 0;
int yspeed = 0;

//ENDSTOP STUFF
EvoEndstop x1(39);
EvoEndstop x2(41);
EvoEndstop y1(43);
EvoEndstop y2(45);

//OTHER
String input = "";

void setup() {
  Serial.begin(9600);
  Serial.println("SETUP COMPLETE");
  //MOTOR SETUP
  xStepper.setMaxSpeed(1000.0);
  yStepper.setMaxSpeed(1000.0);
  xStepper.setAcceleration(25.0);
  yStepper.setAcceleration(25.0);
  xStepper.setSpeed(xspeed);
  yStepper.setSpeed(yspeed);
  //ENDSTOP SETUP
  
}

void loop() {
  readSerial();
  if(parse(input)){
    input = "";
  }
  checkAll();
  updateVelocities();
  moveMotors();
}

void updateVelocities() {
  if(x1.pressed() || x2.pressed()){
    Serial.println("--XX--");
    Serial.println(x1.pressed());
    Serial.println(x2.pressed());
    xspeed = xspeed * -1;
    xStepper.setSpeed(xspeed);
  } else if (y1.pressed() || y2.pressed()) {
    Serial.println("--YY--");
    Serial.println(y1.pressed());
    Serial.println(y2.pressed());
    yspeed = yspeed * -1;
    yStepper.setSpeed(yspeed);
  }
}

void checkAll() {
  extruder.check();
  x1.check();
  x2.check();
  y1.check();
  y2.check();
}

//parses out a given command and updates the needed values
//returns true iff a valid command has been executed
bool parse(String input) {
  if(input.length() == 8 && sanitizeMotorInstruction()){
      Serial.println(input);
      String xstr = input;
      String ystr = input;
      xstr.remove(4,4);
      ystr.remove(0,4);
      xspeed = xstr.toInt();
      yspeed = ystr.toInt();
      xStepper.setSpeed(xspeed);
      yStepper.setSpeed(yspeed);
      return true;
  } else if(input.length() == 8) {
    return true;
  } else if(input == "e"){ //activate extuder
    extruder.extrude();
    return true;
  } else if(input == "p"){ //pause extruder
    extruder.pause();
    return true;
  } else {
  return false;
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

void moveMotors() {
  xStepper.runSpeed();
  yStepper.runSpeed();
}
//*********** ENDSTOP FUNCTIONS ************


