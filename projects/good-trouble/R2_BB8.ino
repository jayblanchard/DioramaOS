/*
 * Name:    Good Trouble - R2D2 & BB8 Someplace They Shouldn't Be
 * Author:  Jay Blanchard
 * Date:    May 2025
 * Board:   Arduino Nano
 * 
 * Usage:   Trigger sound modules robbed from toy driods via 4066 switches driven by
 *          Ardiono Nano and animate lighting
 */

// ---------- User Defined Inputs ----------
unsigned long currentMillis;
const byte totalSwitches = 4; // each droid has 4 switches
// R2
const byte R2Pins[totalSwitches] = {2,3,4,5}; // pins assigned to R2
boolean R2SwitchState[totalSwitches] =   {0,0,0,0}; // What is the current switch state -- boolean is ON or OFF
unsigned int R2SwitchStateOn[totalSwitches] = {0,0,0,0}; // used to store ON times, using unsigned int to hold time up to 65535 ms
unsigned int R2SwitchStateOff[totalSwitches] ={0,0,0,0}; // used to store OFF times, using unsigned int to hold time up to 65535 ms
unsigned long R2PreviousMillis[totalSwitches] = {0,0,0,0}; // What time was the LED last turned OFF -- Needs to store largest possible value (4 bytes used)
// BB8
const byte BB8Pins[totalSwitches] = {18,19,20,21}; // pins assigned to BB8
boolean BB8SwitchState[totalSwitches] =   {0,0,0,0}; // What is the current switch state -- boolean is ON or OFF
unsigned int BB8SwitchStateOn[totalSwitches] = {0,0,0,0}; // used to store ON times, using unsigned int to hold time up to 65535 ms
unsigned int BB8SwitchStateOff[totalSwitches] ={0,0,0,0}; // used to store OFF times, using unsigned int to hold time up to 65535 ms
unsigned long BB8PreviousMillis[totalSwitches] = {0,0,0,0}; // What time was the LED last turned OFF -- Needs to store largest possible value (4 bytes used)

// ---------- Setup  ----------
void setup(){
  // initialize pins as output
  // R2
  for (byte R2CurrentSwitch=0; R2CurrentSwitch<totalSwitches; ++R2CurrentSwitch)
  {
    pinMode(R2Pins[R2CurrentSwitch], OUTPUT);
  }
  // BB8
  for (byte BB8CurrentSwitch=0; BB8CurrentSwitch<totalSwitches; ++BB8CurrentSwitch)
  {
    pinMode(BB8Pins[BB8CurrentSwitch], OUTPUT);
  }
}

// ---------- Main Loop ----------
void loop(){
  currentMillis = millis(); // Get the current time
  
  // R2
  for (byte R2CurrentSwitch=0; R2CurrentSwitch<totalSwitches; ++R2CurrentSwitch) 
  {  
    if (currentMillis - R2PreviousMillis[R2CurrentSwitch] > (R2SwitchState[R2CurrentSwitch] ? R2SwitchStateOff[R2CurrentSwitch]:R2SwitchStateOn[R2CurrentSwitch]))
    {
      digitalWrite(R2Pins[R2CurrentSwitch], R2SwitchState[R2CurrentSwitch] = !R2SwitchState[R2CurrentSwitch]); // flip the state and output it
      (R2SwitchState[R2CurrentSwitch] == 0) ? R2SwitchStateOn[R2CurrentSwitch]=random(400, 800) : R2SwitchStateOff[R2CurrentSwitch]=random(800, 1400);
      R2PreviousMillis[R2CurrentSwitch] = currentMillis;  // reset the time
    }
   }
  // BB8
  for (byte BB8CurrentSwitch=0; BB8CurrentSwitch<totalSwitches; ++BB8CurrentSwitch) 
  {  
    if (currentMillis - BB8PreviousMillis[BB8CurrentSwitch] > (BB8SwitchState[BB8CurrentSwitch] ? BB8SwitchStateOff[BB8CurrentSwitch]:BB8SwitchStateOn[BB8CurrentSwitch]))
    {
      digitalWrite(BB8Pins[BB8CurrentSwitch], BB8SwitchState[BB8CurrentSwitch] = !BB8SwitchState[BB8CurrentSwitch]); // flip the state and output it
      (BB8SwitchState[BB8CurrentSwitch] == 0) ? BB8SwitchStateOn[BB8CurrentSwitch]=random(600, 1200) : BB8SwitchStateOff[BB8CurrentSwitch]=random(1000, 1800);
      BB8PreviousMillis[BB8CurrentSwitch] = currentMillis;  // reset the time
    }
   }
}
