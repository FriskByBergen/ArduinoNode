/* code for evalution made for the Frisk Luft project at www.friskby.no
Based on code from http://www.howmuchsnow.com/arduino/airquality/
The code assume a Sharp dust sensor  */

// include stuff for event interrupts
#include <Event.h>
#include <Timer.h>
Timer t;

int measurePin = 0; //Connect dust sensor to Arduino A0 pin
int ledPower = 5;   //Connect 3 led driver pins of dust sensor to Arduino D5

int samplingTime = 280; // ms that the resistor will heat
int deltaTime = 40; // ms that the resistor will heat after measurement

float voMeasured = 0;
float calcVoltage = 0;

// function that will be triggered by a timer every xx second
void readDust() {
  digitalWrite(ledPower, LOW); // power on the LED
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin); // read the dust value
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH); // turn the LED off

  calcVoltage = voMeasured * (5.0 / 1023.0);
  Serial.println();
  Serial.print("Raw Signal Value (0-1023): ");
  Serial.print(voMeasured);
  Serial.print(" - Voltage: ");
  Serial.println(calcVoltage);
}

void setup() {
  pinMode(ledPower, OUTPUT);
  Serial.begin(115200);
  int dustevent = t.every(1000, readDust);
}

void loop() {
  t.update();
}
