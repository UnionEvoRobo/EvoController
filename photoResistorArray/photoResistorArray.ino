 // pin assignments
 int r1 = 0;
 int r2 = 1;
 int r3 = 2;
 int r4 = 3;
 int r5 = 4;
 int r6 = 5;
 int r7 = 6;
 int r8 = 7;
 const int numResistors = 8;
 int resistors[numResistors] = {r1,r2,r3,r4,r5,r6,r7,r8};
 
 // initialize the serial port
 // and declare inputs and outputs
 void setup() {
  for(int i = 0; i<numResistors; i++){
       pinMode(resistors[i], INPUT);
  }
  Serial.begin(9600);
 }
 
 // read from the analog input connected to the LDR
 // and print the value to the serial port.
 // the delay is only to avoid sending so much data
 // as to make it unreadable.
 void loop() {
  int result[numResistors] = {};
  for(int i = 0; i < numResistors; i++){
    int val = analogRead(resistors[i]);
    result[i] = val;
  }
  String results = "(";
  for(int x = 0; x < numResistors; x++){
    results += result[x];
    if(x != numResistors-1){
      results += ",";
    }
  }
  results += ")";
   Serial.println(results);
   //delay(1000);
 }
