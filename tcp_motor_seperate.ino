#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif
#include <ModbusIP_ESP8266.h>

// MODBUS REGISTERS
const int LED_COIL = 0; // Coil to control LED
const int MOTOR_COIL = 1; // Coil to control motor

// PIN LED
const int ledPin = 13;

// PINS MOTOR
const int motorPin1 = 32; // IN1 on the L298N
const int motorPin2 = 33; // IN2 on the L298N

ModbusIP mb;

void setup() {
  Serial.begin(115200);
  WiFi.begin("nothing1", "somu123456");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());

  mb.server();

  // Initialize LED pin
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  mb.addCoil(LED_COIL);

  // Initialize motor control pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  // Set initial motor state
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  mb.addCoil(MOTOR_COIL);
}

void loop() {
  mb.task();

  // Control LED
  bool ledState = mb.Coil(LED_COIL);
  digitalWrite(ledPin, ledState);

  // Control motor
  bool motorState = mb.Coil(MOTOR_COIL);
  if (motorState) {
    // Motor ON - Forward direction
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
  } else {
    // Motor OFF
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
  }

  delay(10);
}
