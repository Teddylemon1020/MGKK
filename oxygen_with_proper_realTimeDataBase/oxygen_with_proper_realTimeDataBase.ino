#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "DFRobot_OxygenSensor.h"

// Insert your network credentials
#define WIFI_SSID "SO2"
#define WIFI_PASSWORD "12345678"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDIi6RATjXOxB5lHs5-__If9WAMxzZ8aFs"

// Insert RTDB URL
#define DATABASE_URL "https://ardesp32-4012b-default-rtdb.asia-southeast1.firebasedatabase.app"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

// Oxygen sensor configuration
#define Oxygen_IICAddress ADDRESS_3
#define COLLECT_NUMBER  10

DFRobot_OxygenSensor oxygen;

// Define PWM pin for the fan
const int PWM_PIN = 26; // Change this to your PWM pin

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Firebase configuration
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase authentication successful");
    signupOK = true;
  }
  else {
    Serial.printf("Firebase authentication failed: %s\n", config.signer.signupError.message.c_str());
  }

  // Set PWM pin for the fan
  pinMode(PWM_PIN, OUTPUT);

  // Oxygen sensor initialization
  while (!oxygen.begin(Oxygen_IICAddress)) {
    Serial.println("I2c device number error !");
    delay(1000);
  }
  Serial.println("I2c connect success !");
}

void loop() {
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // Write the oxygen concentration to the database
    float oxygenData = oxygen.getOxygenData(COLLECT_NUMBER);
    if (Firebase.RTDB.setFloat(&fbdo, "oxygen_sensor/concentration", oxygenData)) {
      Serial.println("Oxygen data sent to Firebase");
      Serial.print("Concentration: ");
      Serial.print(oxygenData);
      Serial.println(" %vol");

      // Calculate fan speed based on oxygen concentration
      int fanSpeed = calculateFanSpeed(oxygenData);

      // Output fan speed using PWM
      analogWrite(PWM_PIN, fanSpeed);

      // Print fan speed for debugging
      Serial.print("Fan Speed: ");
      Serial.println(fanSpeed);
    }
    else {
      Serial.println("Failed to send oxygen data to Firebase");
      Serial.println("Reason: " + fbdo.errorReason());
    }
  }

  delay(5000); // Adjust delay time according to your needs
}

// Define fuzzy rules for calculating fan speed based on oxygen concentration
int calculateFanSpeed(float oxygen) {
  // Define linguistic variables
  #define LOW_OXYGEN 21.2
  #define MEDIUM_OXYGEN 21.3

  if (oxygen <= LOW_OXYGEN) {
    return 255; // Maximum fan speed
  } else if (oxygen > LOW_OXYGEN && oxygen <= MEDIUM_OXYGEN) {
    // Linear interpolation for fan speed between LOW_OXYGEN and MEDIUM_OXYGEN
    return map(oxygen, LOW_OXYGEN, MEDIUM_OXYGEN, 255, 0);
  } else {
    return 0; // Fan off
  }
}