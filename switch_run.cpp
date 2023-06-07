#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLpzWH-HCG"
#define BLYNK_TEMPLATE_NAME "Curtain Motor"
#define BLYNK_AUTH_TOKEN "zfVPDSPDklJsDSMSw0Yq-lZpL7EX0YQ1"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "zfVPDSPDklJsDSMSw0Yq-lZpL7EX0YQ1";
char ssid[] = "P";
char pass[] = "12121212";

const int TOUCH_PIN = D1;   // Define the touch sensor pin
const int RELAY_PIN = D2;   // Define the relay pin
const int LED_PIN = D3;     // Define the LED pin

bool touchState = false;    // Initialize the touch sensor state to "not touched"
bool relayState = false;    // Initialize the relay state to "off"
bool ledState = false;      // Initialize the LED state to "off"
unsigned long touchTime = 0;  // Initialize the touch time

BlynkTimer timer;

void setup() {
  Serial.begin(9600);
  pinMode(TOUCH_PIN, INPUT);   // Set the touch sensor pin as input
  pinMode(RELAY_PIN, OUTPUT);  // Set the relay pin as output
  pinMode(LED_PIN, OUTPUT);    // Set the LED pin as output

  Blynk.begin(auth, ssid, pass);

  timer.setInterval(100L, checkTouchSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}

void checkTouchSensor() {
  if (digitalRead(TOUCH_PIN) == HIGH) {  // If the touch sensor is touched
    if (!touchState) {
      touchState = true;     // Set the touch sensor state to "touched"
      touchTime = millis();  // Record the touch time
    }
  }
  else {  // If the touch sensor is not touched
    if (touchState) {  // If the touch sensor was previously touched
      touchState = false;   // Set the touch sensor state to "not touched"
      unsigned long duration = millis() - touchTime;  // Calculate the touch duration
      if (duration > 100) {   // If the touch duration is less than 1 second
        ledState = !ledState;   // Toggle the LED state
        digitalWrite(LED_PIN, ledState);   // Turn the LED on or off
        relayState = !relayState;  // Toggle the relay state
        digitalWrite(RELAY_PIN, relayState); // Turn the relay on or off
        Blynk.virtualWrite(V1, relayState);  // Update the virtual switch state in Blynk app
      }
      else {  // If the touch duration is 1 second or longer
        ledState = false;   // Keep the LED off
        digitalWrite(LED_PIN, ledState);   // Turn the LED off
        relayState = false;  // Keep the relay off
        digitalWrite(RELAY_PIN, relayState); // Turn the relay off
        Blynk.virtualWrite(V1, relayState);  // Update the virtual switch state in Blynk app
      }
    }
  }
}

BLYNK_WRITE(V1) {  // Handle virtual switch widget in Blynk app
  relayState = param.asInt();
  digitalWrite(RELAY_PIN, relayState);
  Blynk.virtualWrite(V1, relayState);  // Update the virtual switch state in Blynk app
}
