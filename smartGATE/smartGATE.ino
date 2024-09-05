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

 if (digitalRead(limitA) == LOW){   //set to HIGH if the trigger is utilizes HIGH insted of LOW
  digitalWrite(ArminA, HIGH);
  digitalWrite(ArminB, LOW);
  
  delay(500);


 
 } 
 else if  (digitalRead(limitA) == HIGH) {
  digitalWrite(ArminA, LOW); //set to HIGH if the actuator utilizes HIGH for no movement
  digitalWrite(ArminB, LOW); //set to HIGH if the actuator utilizes HIGH for no movement
 
  
 }
 if (digitalRead(limitB) == LOW){

  digitalWrite(LocoA, HIGH);
  digitalWrite(LocoB, LOW);

  delay(500);

  
 } 
 else if (digitalRead(limitB) == HIGH) {
  digitalWrite(LocoA, LOW);
  digitalWrite(LocoB, LOW);


  
 }

}

void closinggate() {
  BTserial.println("closing the gate");

  digitalWrite(ArminA, LOW);
  
  digitalWrite(ArminB, HIGH);

  digitalWrite(LocoA, LOW);
  
  digitalWrite(LocoB, HIGH);
  
  delay(10000);
  digitalWrite(ArminA, LOW);
  digitalWrite(ArminB, LOW);
  digitalWrite(LocoA, LOW);
  digitalWrite(LocoB, LOW);

}





