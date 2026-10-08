/*
 * Name:    STTOS - Galileo Shuttlecraft
 * Author:  Jay Blanchard
 * Date:    January 2021
 * Board:   Arduino Nano
 * 
 * Usage:   Load into Ardiono Nano to animate lighting for the STTOS Galileo Shuttlecraft
 *          Bussards
 */

/* Bussard Spinner Variables */
const int ledPin[] = {2, 4, 7};
const int numLED = sizeof(ledPin) / sizeof(ledPin[0]);

/* 
 * Bussard Color Wave Variables 
 * Designed for a common cathode RGB LED the actual color of the LED does not have to match
 * the pin name as long as each of the three anode (color) pins are attached to the proper
 * output on the Arduino. The only time you really need to pay attention is if you want the
 * RGB LEDs to be on the same "cycle" and showing the same color at the same time. 
 */
const int redA = 3;
const int greenA = 5;
const int blueA = 6;

const int redB = 11;
const int greenB = 10;
const int blueB = 9;

long int fade_goTime;
int fade_break = 2; // color change speed, lower number means faster change

void setup() {
  /* Bussard Spinner Set Up */
  Serial.begin(9600);
  for (byte i = 0; i < numLED; i++) {
    pinMode(ledPin[i], OUTPUT);
    digitalWrite(ledPin[i], LOW);
  }
  
  /* Bussard Color Wave Set Up */
  fade_goTime = millis();
}

void loop() {
  bussardSpinner();
  if(millis() >= fade_goTime) bussardWave();
}


void bussardSpinner() {

  const unsigned long delayTime = 175; // speed of spinner
  static unsigned long lastTime;
  static int idx = 0;        // current index into led array
  static int prevIdx = numLED - 1;

  unsigned long t = millis();
  if (t - lastTime >= delayTime) {
    lastTime = t;
    digitalWrite(ledPin[idx], HIGH);
    digitalWrite(ledPin[prevIdx], LOW);
    prevIdx = idx;
    idx++;

    if (numLED == idx) {
      idx = 0; // reset idx
    }

  }
}

void bussardWave() {
 
  static unsigned int rgbColour[3] = {255,0,0};
  static int incColour = 1;
  static int decColour = 0;
  static int i = -1;
     
  // cross-fade two colours.
  i++;
  if(i > 254) {
    i=0;
    decColour ++;
    if(decColour > 2) decColour = 0;     
    if(decColour == 2)  incColour = 0;
    else incColour = decColour +1;
  }

       
  rgbColour[decColour] -= 1;
  rgbColour[incColour] += 1;
       
  setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);     
  fade_goTime = millis() + fade_break;       
}

void setColourRgb(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(redA, red);
  analogWrite(greenA, green);
  analogWrite(blueA, blue);
  analogWrite(redB, red);
  analogWrite(greenB, green);
  analogWrite(blueB, blue);
 }
