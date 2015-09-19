/* code for evalution made for the Frisk Luft project at www.friskby.no
*/

#include <LLAPSerial.h>
#include <DHT.h>

int measurePin = 0; //Connect dust sensor to Arduino A0 pin
int ledPower = 5;   //Connect 3 led driver pins of dust sensor to Arduino D5

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

#define DEVICEID "DS"	// this is the LLAP device ID
#define DHTPIN 4     // what I/O the DHT-22 data pin is connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(ledPower, OUTPUT);
  Serial.begin(115200);
  pinMode(8, OUTPUT);		// switch on the radio
  digitalWrite(8, HIGH);
  pinMode(4, OUTPUT);		// switch on the radio
  digitalWrite(4, LOW);	// ensure the radio is not sleeping
  delay(1000);				// allow the radio to startup
  LLAP.init(DEVICEID);
  dht.begin();
  LLAP.sendMessage(F("STARTED"));
}

void loop() {

  // every 10 seconds
  static unsigned long lastTime = millis();
  if (millis() - lastTime >= 10000)
  {
    lastTime = millis();
    digitalWrite(ledPower, LOW); // power on the LED
    delayMicroseconds(samplingTime);
    voMeasured = analogRead(measurePin); // read the dust value
    delayMicroseconds(deltaTime);
    digitalWrite(ledPower, HIGH); // turn the LED off
    delayMicroseconds(sleepTime);

    // 0 - 5V mapped to 0 - 1023 integer values
    calcVoltage = voMeasured * (5.0 / 1023.0);
    int h = dht.readHumidity() * 10;
    int t = dht.readTemperature() * 10;
    int d = voMeasured; 
    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t) || isnan(h) || isnan(d)) {
      LLAP.sendMessage(F("ERROR"));
    } 
    else {
      LLAP.sendIntWithDP("HUM", h, 1);
      LLAP.sendIntWithDP("TMP", t, 1);
      LLAP.sendInt("DST", d);
    }
  }
  
  // test if we have received anything on LLAP
  if (LLAP.bMsgReceived) {
    Serial.print(F("msg:"));
    Serial.println(LLAP.sMessage);
    LLAP.bMsgReceived = false;	// if we do not clear the message flag then message processing will be blocked
  }
}
