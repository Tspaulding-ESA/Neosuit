#include <IRremote.hpp>
#include <Servo.h>
#include <LedControl.h>

/* Variables, Pins*/
#define LED_COUNT 7
#define IR_RECEIVE_PIN 11  // Pin connected to the IR sensor
#define eyes 2
#define helmet 3
#define bracers 4
#define chest 5
#define belt 6
#define legs 7
#define misc 8


// LED vars
int ledPins[LED_COUNT] = {eyes, helmet, bracers, chest, belt, legs, misc};

// Added from https://forum.arduino.cc/t/how-to-make-2-leds-blink-at-the-same-time/366237/7
int dur;  // duration of blink

//Servo Angles
int angle1;  //angle of servo1
int angle2;  //angle of servo2

/*-----( Declare objects )-----*/
Servo helmet1;
Servo helmet2;


unsigned long irRawdata;  //variable for storing IR data
unsigned long last_irData;

void setup() {
  IrReceiver.begin(IR_RECEIVE_PIN);  // Start the IR receiver

  Serial.begin(9600);
  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);

  helmet1.attach(9);   //Right
  helmet2.attach(10);  //Left
  helmet1.write(180);   //Right Open
  helmet2.write(0);    //Left Open
  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(ledPins[i], OUTPUT);  // Set all LED pins as output
  }
}

void loop() {
  if (IrReceiver.decode())  // have we received an IR signal?
  {
    irRawdata = IrReceiver.decodedIRData.command;
    Serial.println(irRawdata);
    // Check if it is a repeat IR code
    Serial.println(IrReceiver.decodedIRData.flags);
    if (irRawdata == last_irData) {
      //set the current decodedRawData to the last decodedRawData
      irRawdata = last_irData;
    }
    if (irRawdata != last_irData) {
      switch (IrReceiver.decodedIRData.command) {
        case 70:  // VOL+ button pressed Helmet Closes
            for (int i = 0; i < 120; i += 1) {// Helmet Close
              angle1 = 120 - i;
              angle2 = 0 + i;
              helmet1.write(angle1);
              helmet2.write(angle2);
              delay(10);
            }
            for (int dur = 100; dur >= 4; dur -= 15) { //Lights Engage
              Serial.print("duration:");
              Serial.println(dur);
              for (int rep = 0; rep < 11; rep += 1) {
                Serial.print("repetition: ");
                Serial.println(rep);
                if (rep < 9) { // For all reps except the last, flash 
                  digitalWrite(ledPins[0], HIGH);  // Turn on all LEDs
                  digitalWrite(ledPins[1], HIGH);
                  digitalWrite(ledPins[2], HIGH);
                  digitalWrite(ledPins[3], HIGH);
                  digitalWrite(ledPins[4], HIGH);
                  digitalWrite(ledPins[5], HIGH);
                  digitalWrite(ledPins[6], HIGH);
                  delay(1);
                  digitalWrite(ledPins[0], LOW);  // Turn off all LEDs
                  digitalWrite(ledPins[1], LOW);
                  digitalWrite(ledPins[2], LOW);
                  digitalWrite(ledPins[3], LOW);
                  digitalWrite(ledPins[4], LOW);
                  digitalWrite(ledPins[5], LOW);
                  digitalWrite(ledPins[6], LOW);
                  delay(dur);
                }
                if (rep == 10) { //For the last rep, end ON
                  digitalWrite(ledPins[0], HIGH);  // Turn on all LEDs
                  digitalWrite(ledPins[1], HIGH);
                  digitalWrite(ledPins[2], HIGH);
                  digitalWrite(ledPins[3], HIGH);
                  digitalWrite(ledPins[4], HIGH);
                  digitalWrite(ledPins[5], HIGH);
                  digitalWrite(ledPins[6], HIGH);
                  //delay (5000);
                  //digitalWrite(ledPins[0], LOW);  // Turn the eyes off
                }
              }
            }
            break;
        case 21:  // VOL- button pressed Helmet Opens
            for (int dur = 2; dur < 200; dur += 35) { //Lights Disengage
              Serial.print("duration: ");
              Serial.println(dur);
              for (int rep = 10; rep >= 0; rep -= 1) {
                Serial.print("repetition: ");
                Serial.println(rep);
                if (rep > 0) {// For most reps flash lights
                  digitalWrite(ledPins[0], HIGH);  // Turn on all LEDs
                  digitalWrite(ledPins[1], HIGH);
                  digitalWrite(ledPins[2], HIGH);
                  digitalWrite(ledPins[3], HIGH);
                  digitalWrite(ledPins[4], HIGH);
                  digitalWrite(ledPins[5], HIGH);
                  digitalWrite(ledPins[6], HIGH);
                  delay(1);
                  digitalWrite(ledPins[0], LOW);  // Turn off all LEDs
                  digitalWrite(ledPins[1], LOW);
                  digitalWrite(ledPins[2], LOW);
                  digitalWrite(ledPins[3], LOW);
                  digitalWrite(ledPins[4], LOW);
                  digitalWrite(ledPins[5], LOW);
                  digitalWrite(ledPins[6], LOW);
                  delay(dur);
                }
                if (rep == 0) { //For final rep, turn lights off
                  digitalWrite(ledPins[0], LOW);  // Turn off all LEDs
                  digitalWrite(ledPins[1], LOW);
                  digitalWrite(ledPins[2], LOW);
                  digitalWrite(ledPins[3], LOW);
                  digitalWrite(ledPins[4], LOW);
                  digitalWrite(ledPins[5], LOW);
                  digitalWrite(ledPins[6], LOW);
                }
              }
            }
            for (int i = 0; i < 120; i += 1) {// Helmet Open
              angle1 = 60 + i;
              angle2 = 120 - i;
              helmet1.write(angle1);
              helmet2.write(angle2);
            }
            break;
          }
          delay(5000);
      };
    }
    //store the last decodedRawData
    last_irData = irRawdata;
    IrReceiver.resume();  // receive the next value
}