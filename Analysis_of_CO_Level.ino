// I used Mega2560, for any other Arduino, please check and change pin numbers

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
/*
 * The LCD circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VDD pin to +5V
 * LCD LED- to ground
 * 220 resistor: one end to +5V and other to LCD LED+
 * 4.7K resistor: one end to +5V and other to LCD V0
 */

const int analogOP = 13;
const int analogIP = A0;
/*
* The MQ-7 circuit:
* MQ-7 VCC to pin 13 (PWM)
* MQ-7 GND to GND
* MQ-7 A0 to A0
*/

const String wifiName = "Hotspot";
const String wifiPassword = "uknowmypassword";
const String serverAddr = "192.168.43.76";
const String serverPort = "80";
/*
 * ESP-01 VCC and CH_PD to 3.3.V
 * ESP-01 GND to ground
 * ESP-01 RXD to TXD
 * ESP-01 TXD to RXD
 */


void setup() {
  // put your setup code here, to run once:
  // initialise wifi module (conneccting to AP)
  esp_init();
  // initialise LCD module
  lcd.begin(16,2);
  // setting I/P and O/P
  pinMode(analogOP, OUTPUT);
  pinMode(analogIP, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  heatSensor();
  double reading = takeReading();
  // send reading
  sendReading(reading);
  //display on LCD
  lcd.clear();
  lcd.print("CO is "+String(reading)+" ppm");
}

void heatSensor() {
  // lets heat the sensor
  analogWrite(analogOP, 255);
  // wait for 1 minute
  for(int i = 0; i < 60; i ++) {
    delay(1000);
  }
}

double takeReading() {
  // switch to 1.4 supply - (255/5) * 1.4 = 71.4
  analogWrite(analogOP, 72);
  delay(100);
  // take reading
  double reading = analogRead(analogIP);
  // convert to voltage
  reading = reading * 5 / 4095;
  // apply formula
  reading = 3.027 * exp(1.0698 * reading);
  return reading;
}

void esp_init() {
  Serial.begin(115200);
  delay(1000);
  Serial.print("AT+CWJAP=\""+wifiName+"\",\""+wifiPassword+"\"\r\n");
  delay(2000);
}

void sendReading(double reading) {
  String data = String(reading);
  // we have set loc to 201, by default
  // below is http request to be sent through a TCP connection
  String httpRequest = "GET /prj/logger.php?loc=201&value="+data+" HTTP/1.1\r\nHost: "+serverAddr+"\r\n\r\n";
  int dlen = httpRequest.length();
  // start a TCP connection - AT+CIPSTART="TCP","192.168.43.76",80
  Serial.print("AT+CIPSTART=\"TCP\",\""+serverAddr+"\","+serverPort+"\r\n");
  // we are quick prototyping, so lets skip response (provided everything goes good)
  // a delay to skip response to avoid collision
  delay(300);

  // send AT+CIPSEND
  Serial.print("AT+CIPSEND="+String(dlen)+"\r\n"); 
  delay(500); 
  Serial.print(httpRequest);
}

