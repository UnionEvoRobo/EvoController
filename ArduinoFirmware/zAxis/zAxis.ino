#include <EvoLib.h>
#include <AccelStepper.h>

#include <avr/wdt.h>
#include <stdio.h>
#include <ctype.h>

//MOTOR SETUP
#define STEPS 200
AccelStepper zStepper(AccelStepper::FULL4WIRE,2,3,4,5);
int zSpeed = 100;

//ENDSTOP SETUP
EvoEndstop z1(6);

//PARSING GLOBAL VARIABLES
String input = "";

void setup() {

  //SERIAL SETUP
  Serial.begin(9600);

  //MOTOR SETUP
  zStepper.setMaxSpeed(1000.0);
  zStepper.setAcceleration(25.0);
  zStepper.setSpeed(zSpeed);
  //Serial.println("S");
}

void loop() {
  //Read
  readSerial();

  //Eval & Execute
  input = parse(input);
}

//parses out a given command and updates the needed values
//returns true iff a valid command has been executed
String parse(String inputStr) {
  if(input == "u"){
    raise();
  } else if(input == "d"){
    lower();
  }
  return "";
}

//reads a bit from serial and adds it to the input string
void readSerial() {
  if(Serial.available() > 0){
    input += (char)Serial.read();
  }
}

//*********** MOTOR FUNCTIONS ************
void raise(){
  zStepper.setSpeed(-75);
  int starttime = millis();
  int endtime = starttime;
  while ((endtime - starttime) <=20000) // do this loop for up to 1000mS
  {
    zStepper.runSpeed();
    endtime = millis();
  }
  Serial.print('u');
}

void lower(){
  zStepper.setSpeed(100);
  while(!z1.pressed()){
    zStepper.runSpeed();
  }
  Serial.print('d');
}

