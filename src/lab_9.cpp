/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/Abdurrahman/Documents/labs/lab_9/src/lab_9.ino"
#include "SparkFun_VCNL4040_Arduino_Library.h"
#include <Wire.h>
#include "oled-wing-adafruit.h"
#include <blynk.h>
void setup();
void flash(int time);
void loop();
#line 5 "c:/Users/Abdurrahman/Documents/labs/lab_9/src/lab_9.ino"
SYSTEM_THREAD(ENABLED);
OledWingAdafruit display;
VCNL4040 proximitySensor;

bool setPoint = false;
bool setHighPoint = false;
u_int16_t lowPoint = 2000;
u_int16_t highPoint = 5000;
int pinData;

// setup() runs once, when the device is first turned on.
void setup() {
  Wire.begin();
  display.setup();
	display.clearDisplay();
	display.display();
  pinMode(D5,OUTPUT);
  pinMode(D6,OUTPUT);
  pinMode(D7,OUTPUT);
  pinMode(D8,INPUT);
  proximitySensor.begin();
  proximitySensor.powerOffProximity();
  proximitySensor.powerOnAmbient();
  Serial.begin(9600);
  Blynk.begin("H3gXxetGr9r8nia_NkEMpEx44ppDEStX", IPAddress(167, 172, 234, 162), 9090);

}

//so the button on blynk works
BLYNK_WRITE(V2){
  pinData = param.asInt();
}

// used for the sos light
void flash(int time){
  digitalWrite(D6,HIGH);
    delay(time);
    digitalWrite(D6,LOW);
    delay(time);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  display.loop();
  Blynk.run();
  int ambientValue = proximitySensor.getAmbient();
  Serial.println(ambientValue);

  // mode A
  // if A pressed display low and high point and allow the user to change them
  if(display.pressedA()){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Low Point: ");
    display.println(lowPoint);
    display.print("High Point: ");
    display.println(highPoint);
    display.println("Press the button to \nchange");
    display.display();
    setPoint = true;
  }

  // code for changing the low and high point
  if (digitalRead(D8) == true && setPoint == true){ // to change low pint
    delay(1000);
    while (digitalRead(D8) == false){ // to display the value on the Potentiometer on the oled display while changing low point
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("Choose low setting point");
      display.println(analogRead(A4));
      display.display();
      delay(200);
    }
    lowPoint = analogRead(A4);
    setPoint = false;
    setHighPoint = true;
  }
  if (setPoint == false && setHighPoint == true){ // to change high point
    delay(1000);
    while (digitalRead(D8) == false){ // to display the value on the Potentiometer on the oled display while changing high point
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Choose high setting point");
      display.println(analogRead(A4));
      display.display();
      delay(200);
    }
    highPoint = analogRead(A4);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Low Point: ");
    display.println(lowPoint);
    display.println("High Point: ");
    display.println(highPoint);
    display.display();
    delay(5000);
    display.clearDisplay();
    display.display();
    setPoint = false;
    setHighPoint = false;
  }

  // to flash the leds depending on the low and high point of light level
  if(ambientValue < lowPoint && digitalRead(D7) == false){ // flash blue if light level is below low point
    Blynk.notify("light level lower than low point");
    digitalWrite(D5,LOW);
    digitalWrite(D6,LOW);
    digitalWrite(D7,HIGH);
  }
  if (lowPoint <= ambientValue && ambientValue <= highPoint && digitalRead(D5) == false){ // flash green if light level is between low and high point
    Blynk.notify("light level between low and high point");
    digitalWrite(D6,LOW);
    digitalWrite(D5,HIGH);
    digitalWrite(D7,LOW);
  }
  if (ambientValue > highPoint && digitalRead(D6) == false){ // flash green if light level is above high point
    Blynk.notify("light level above high point");
    digitalWrite(D7,LOW);
    digitalWrite(D6,HIGH);
    digitalWrite(D5,LOW);
  }

  // mode B
  // calculate the temperature in celsius and fahrenheit
  uint64_t reading = analogRead(A5);
  double voltage = (reading * 3.3) / 4095.0;
  double temperature = (voltage - 0.5) * 100;
  double fahrenheit = temperature * 1.8 + 32;
  
  //preparing the message for the oled display
  String strCelsius = String(temperature); 
  String strFahrenheit = String(fahrenheit);
  String textForOledDisplay = "Your temerature is:\ncelsius: "+ strCelsius + "\nfahrenheit: " + strFahrenheit;

  // send the temperature in celsius and fahrenheit to blynk
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, fahrenheit);

  // display the temperature in celsius and fahrenheit on the oled display if B pressed
   if (display.pressedB()) {
    display.clearDisplay();
		display.setTextSize(1);
		display.setTextColor(WHITE);
		display.setCursor(0,0);
    display.println(textForOledDisplay);
		display.display();
    setPoint = false;
  }

  // if the button on blynk is pressed flash the SOS light
  if(pinData == 1){
    delay(300);
    digitalWrite(D7,LOW);
    digitalWrite(D5,LOW);
    digitalWrite(D6,LOW);
    flash(200);
    flash(200);
    flash(200);
    delay(300);
    flash(500);
    flash(500);
    flash(500);
    flash(200);
    flash(200);
    flash(200);
    delay(1000);
  }

}