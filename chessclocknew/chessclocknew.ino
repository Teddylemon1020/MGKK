#include <Arduino.h>
#include <SevSeg.h> // Include the SevSeg library

const int Button1 = 2; 
const int Button2 = 3;
const int BuzzerPin = 4;
const int Light1 = 5;
const int Light2 = 6;
const unsigned long gameTimeout = 9000; // 9 seconds
const unsigned long indicatorDuration = 10000; // 10 seconds

unsigned long lastPressTime = 0; // Time of the last button press
unsigned long indicatorStartTime = 0; // Time when the indicators were activated
int remainingTime = 9; // Global variable for remaining time
bool player1Turn = true; // Start with Player 1's turn
bool gameStarted = false; // Track if the game has started
bool timeExpired = false; // Track if the time has expired

SevSeg sevseg; // Instantiate a SevSeg object

void setup() {
  pinMode(Button1, INPUT_PULLUP);
  pinMode(Button2, INPUT_PULLUP);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(Light1, OUTPUT);
  pinMode(Light2, OUTPUT);
  digitalWrite(BuzzerPin, LOW);
  digitalWrite(Light1, LOW);
  digitalWrite(Light2, LOW);
  Serial.begin(9600);
  
  // Initialize the seven-segment display
  byte numDigits = 1;
  byte digitPins[] = {7}; // Adjust this pin according to your wiring
  byte segmentPins[] = {8, 9, 10, 11, 12, 13, A0}; // Adjust these pins according to your wiring
  bool resistorsOnSegments = true; // True if resistors are on the segment pins
  bool updateWithDelays = false; // Set to false for non-blocking updates
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90); // Set the brightness of the display
  sevseg.setNumber(9); // Set the initial display to 9
}

void loop() {
  if (!gameStarted) {
    StartGame();
  } else {
    if (player1Turn) {
      Player1();
    } else {
      Player2();
    }
  }
  
  // Update the seven-segment display
  sevseg.refreshDisplay(); 
  
  if (timeExpired) {
    ManageIndicators(); // Handle the buzzer and light after time expires
  }
}

void StartGame() {
  // Check if Player 1 presses their button to start the timer for Player 2
  if (digitalRead(Button1) == LOW) {
    gameStarted = true; // Game starts
    player1Turn = false; // Switch to Player 2's turn
    lastPressTime = millis(); // Initialize Player 2's timer
    remainingTime = 9; // Reset remaining time to 9 seconds
    timeExpired = false; // Reset the expired flag
    sevseg.setNumber(9); // Set the display to 9 seconds
    Serial.println("Game started. Player 2's turn.");
  }
}

void Player1() {
  UpdateTimer(); // Update the timer for Player 1

  if (timeExpired) {
    // Time expired, waiting for Player 1 to acknowledge
    if (digitalRead(Button1) == LOW) {
      // Acknowledge the expired turn
      player1Turn = false; // Switch to Player 2's turn
      ResetIndicators();
      lastPressTime = millis();
      remainingTime = 9; // Reset remaining time to 9 seconds
      timeExpired = false; // Reset the expired flag
      Serial.println("Player 1 acknowledged. Player 2's turn.");
    }
  } else if (digitalRead(Button1) == LOW) {
    // Button1 is pressed before time expires, reset the timer and switch turn
    player1Turn = false; // Switch to Player 2's turn
    ResetIndicators();
    lastPressTime = millis();
    remainingTime = 9; // Reset remaining time to 9 seconds
    Serial.println("Player 1 pressed the button. Player 2's turn.");
  }
}

void Player2() {
  UpdateTimer(); // Update the timer for Player 2

  if (timeExpired) {
    // Time expired, waiting for Player 2 to acknowledge
    if (digitalRead(Button2) == LOW) {
      // Acknowledge the expired turn
      player1Turn = true; // Switch to Player 1's turn
      ResetIndicators();
      lastPressTime = millis();
      remainingTime = 9; // Reset remaining time to 9 seconds
      timeExpired = false; // Reset the expired flag
      Serial.println("Player 2 acknowledged. Player 1's turn.");
    }
  } else if (digitalRead(Button2) == LOW) {
    // Button2 is pressed before time expires, reset the timer and switch turn
    player1Turn = true; // Switch to Player 1's turn
    ResetIndicators();
    lastPressTime = millis();
    remainingTime = 9; // Reset remaining time to 9 seconds
    Serial.println("Player 2 pressed the button. Player 1's turn.");
  }
}

void UpdateTimer() {
    unsigned long elapsedTime = millis() - lastPressTime;
    remainingTime = 9 - elapsedTime / 1000;

    Serial.print("Remaining time: ");
    Serial.println(remainingTime); // Debugging output

    if (remainingTime <= 0) {
        remainingTime = 0;
        if (!timeExpired) {
          timeExpired = true; // Set the expired flag
          indicatorStartTime = millis(); // Record the start time for the indicators
          tone(BuzzerPin, 1000); // Ring the buzzer
          if (player1Turn) {
            digitalWrite(Light1, HIGH); // Indicate Player 1 missed their turn
            Serial.println("Player 1's time expired.");
          } else {
            digitalWrite(Light2, HIGH); // Indicate Player 2 missed their turn
            Serial.println("Player 2's time expired.");
          }
          delay(1000);
        }
    } else {
        sevseg.setNumber(remainingTime); // Update the display with the remaining time
    }
}

void ResetIndicators() {
  noTone(BuzzerPin); // Turn off the buzzer
  digitalWrite(Light1, LOW); // Turn off Player 1's light
  digitalWrite(Light2, LOW); // Turn off Player 2's light
  Serial.println("Indicators reset.");
}

void ManageIndicators() {
  // Keep the indicators on for a fixed duration after time expires
  if (millis() - indicatorStartTime >= indicatorDuration) {
    ResetIndicators();
    timeExpired = false; // Reset the expired flag
  }
}
