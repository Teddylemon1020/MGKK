#include <Arduino.h>
#include <SoftwareSerial.h>


//rx pin 2, tx pin 3
SoftwareSerial BTserial(2,3);

const int ArminA = 6;
const int ArminB = 8;
const int LocoA = 7;
const int LocoB = 9;
const int limitA = 2;
const int limitB = 3;
const int limitC = 4;
const int limitD = 5;




void setup() {
 pinMode(ArminA, OUTPUT);
 pinMode(ArminB, OUTPUT);
 pinMode(LocoA, OUTPUT);
 pinMode(LocoB, OUTPUT);
 pinMode(limitA, INPUT);
 pinMode(limitB, INPUT);
 pinMode(limitC, INPUT);
 pinMode(limitD, INPUT);

 Serial.begin(9600);
 BTserial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
 if (BTserial.available()) {
    char command = BTserial.read();
    Serial.println(command);
    Commandments(command);

    digitalWrite(ArminA, LOW); // set to HIGH if the actuator utilizes HIGH  for no movemtn 
    digitalWrite(ArminB, LOW);
    digitalWrite(LocoA, LOW);
    digitalWrite(LocoB, LOW);
  }
   if (digitalRead(limitA) == LOW){
    STOP();
   }
   if (digitalRead(limitB) == LOW){
    STOP();
   }
   if (digitalRead(limitC) == LOW){
    STOP();
   }
   if (digitalRead(limitC) == LOW){
    STOP();
   }
}

void Commandments(char command){
  switch(command){
    case 'open':
      openninggate();
      break;
    case 'close':
      closinggate();
      break;
  }
}

void openninggate() {
 BTserial.println("openning the gate");

 if ((digitalRead(ArminA) == LOW) && (digitalRead(ArminB) == LOW)){   //set to HIGH if the trigger is utilizes HIGH insted of LOW
  digitalWrite(ArminA, HIGH);
  digitalWrite(ArminB, LOW);
  
  delay(500);


 
 } 
 else{
  digitalWrite(ArminA, LOW); //set to HIGH if the actuator utilizes HIGH for no movement
  digitalWrite(ArminB, LOW); //set to HIGH if the actuator utilizes HIGH for no movement

  delay(500);
 
 }
 if ((digitalRead(LocoA) == LOW) && (digitalRead(LocoB) == LOW)){

  digitalWrite(LocoA, HIGH);
  digitalWrite(LocoB, LOW);

  delay(500);

 } 
 else{
  digitalWrite(LocoA, LOW);
  digitalWrite(LocoB, LOW);

  delay(500);

 }
 BTserial.println("Gate Opened");

 }




void closinggate() {
  BTserial.println("closing the gate");

  if (digitalRead((ArminA) == LOW) && (digitalRead(ArminB) == LOW)){
    digitalWrite(ArminA, LOW);
    digitalWrite(ArminB, HIGH);

    delay(500);
  }

  else{
    digitalWrite(ArminA, LOW);
    digitalWrite(ArminB, LOW);

    delay(500);

  }

  if ((digitalRead(LocoA) == LOW) && (digitalRead(LocoB) == LOW)){
    digitalWrite(LocoA, LOW);
    digitalWrite(LocoB, HIGH);

    delay(500);

  }
  else{
    digitalWrite(LocoA, LOW);
    digitalWrite(LocoB, LOW);
    delay(500);
  }
  BTserial.println("Gate Closed");
  
}

void STOP(){ //THIS PORTION OF THE COMMAND SHOULD BE ADAPTED ON THE HIGH AND LOW OF A L298N MOTOR DRIVER
  digitalWrite(ArminA, LOW);
  digitalWrite(ArminB, LOW);
  digitalWrite(LocoA, LOW);
  digitalWrite(LocoB, LOW);

}





