// Presentation Only Code
// by Atanas Ilinov, Jess Rufus

// Libraries
#include <Servo.h>                // servo library
#include <Adafruit_NeoPixel.h>    // LED ring library
#include <SD.h>                   // SD  card library
#include <TMRpcm.h>               // this library allows us to play wav files from an SD card
#include <SPI.h>                  // need library for SD card module to communicate with the arduino using SPI protocols

int buttonPinOn = 3;              // button that starts the sequence.

Servo servoDoors1;                // create servo object to control front doors servo
Servo servoDoors2;                // create servo object to control back doors servo
int pos = 180;                    // variable to store the servo position
Servo catapault;                  // variable to store catapault servo

// set up LED ring
#define PIN        4              // LED ring
#define NUMPIXELS  44             // number of LEDs on LED ring
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAY 680                 // Time (in milliseconds) to pause between pixels

#define SD_ChipSelectPin 10       // SD card module to pin 10
TMRpcm sound;                     // object to use with sound and SD card module

int sunsetOn = 0;                 // variable that triggers sunset sequence if set to HIGH.

void setup() {
  Serial.begin(9600);             // initialize the serial communication

  pinMode(buttonPinOn, INPUT);    // button for turning on sequence

  servoDoors1.attach(7);          // front doors servo
  servoDoors2.attach(8);          // back doors servo

  catapault.attach(5);                // catapault servo pin
  resetCatapult();

  sound.speakerPin = 9;           // speaker pin
  SD.begin(SD_ChipSelectPin);                   // initialize SD card module
  sound.setVolume(3);             // set sound volume level (0-7)

  pixels.begin();                 // INITIALIZE NeoPixel strip object
}

void loop() {

  // turn off LEDs
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes GRB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));

    pixels.show();     // Send the updated pixel colors to the hardware.
  }
  
  // servoDoors1.write(180);         // set door servos to initial position
  // servoDoors2.write(180);         // set door servos to initial position

  //sunrise sequence
  // read the state of the pushbutton value:
  int buttonState = digitalRead(buttonPinOn);

  if (buttonState == LOW) {       // if the button is not pressed, the buttonState is HIGH:

    // doors open
    for(pos = 0; pos < 180; pos += 10) {   // goes from 180 degrees to 0 degrees in steps of 1 degree                               
      servoDoors1.write(180 - pos);     // front doors servo
      servoDoors2.write(180 - pos);     // back doors servo
      delay(100);
    }

    // hit the drum
    shoot();
    resetCatapult();

    // during the presentation you get one minute to take care of plant (spray it with water)
    delay(6000);

    // if moisture sensor senses increases in humidity, it will drop below 450.
    // if this happens, assume owner has taken care of plant and set sunsetOn variable to high
    if(analogRead(A0) < 450){
      sunsetOn = 1;
    }

    // doors close
    for(pos = 0; pos < 180; pos += 10) {    // goes from 0 degrees to 180 degrees in steps of 1 degree
      servoDoors1.write(pos);              // tell servo to go to position in variable 'pos'
      servoDoors2.write(pos);
      delay(100);                          // waits 15ms for the servo to reach the position
    }

    //sunset sequence
    if(sunsetOn == 1){

      // play ambient sound
      // if (SD.exists("sound.wav")) {          // check if the sound file exists on the SD card
        sound.play("sound.wav");             // play the sound file
      // } else {
      //   Serial.println("Sound file not found."); // print an error message to the serial monitor
      // }

      // turn on LEDs
      for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

        // pixels.Color() takes GRB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(155, 255, 0));

        pixels.show();   // Send the updated pixel colors to the hardware.

        delay(DELAY); // Pause after LED turns on
      }

      // turn off LEDs
      for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

        // pixels.Color() takes GRB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));

        pixels.show();     // Send the updated pixel colors to the hardware.

        delay(DELAY); // Pause after LED turns off
      }
  }
}

void resetCatapult() {
  catapault.write(0);
  delay(500);
}

void shoot() {
  catapault.write(180);
  delay(500);
}

}
