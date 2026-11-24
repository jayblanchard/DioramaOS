/*
 * Name:    Droid in The Dessert - What are we even doing here?
 * Author:  Jay Blanchard
 * Date:    Nov 2026
 * Board:   Arduino Nano
 *
 * Audio:     DFPlayer Mini, BUSY-driven state machine.
 * Animation: 5V 28BYJ-48 + ULN2003, nonblocking half-step sequencing.
 *            Nano: IN1=D2, IN2=D3, IN3=D4, IN4=D5.
 *            Keep the motor on its separate regulated 5V supply, with common GND.
 * 
 * Usage:   The sketch handles the ask and response between R2D2 and C3PO 
 *          where 3PO issues a complaint (randomly) 
 *          and R2 responds (randomly). A motor driver turns R2's dome during that exchange.
 *          Other, non-connected, devices (LEDs) are directly powered.
 *          
 */
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>



// Hardware connections
constexpr uint8_t DFPLAYER_RX_PIN = 10;
constexpr uint8_t DFPLAYER_TX_PIN = 11;
constexpr uint8_t BUSY_PIN       = 12;
constexpr uint8_t BUTTON_PIN     = 7;

constexpr uint8_t MOTOR_PINS[4] = {2, 3, 4, 5};

// Audio tracks in the SD card's /mp3 directory
constexpr uint8_t R2_FIRST   = 1;
constexpr uint8_t R2_LAST    = 10;
constexpr uint8_t C3PO_FIRST = 11;
constexpr uint8_t C3PO_LAST  = 20;

constexpr uint8_t VOLUME = 22;
constexpr unsigned long DEBOUNCE_MS = 40;
constexpr unsigned long START_TIMEOUT_MS = 5000;

// The 28BYJ-48 gearbox is approximately 4096 half-steps/output revolution.
// Calibrate this later if we need precise angles.
constexpr long HALF_STEPS_PER_REV = 4096;
constexpr unsigned long STEP_INTERVAL_US = 2500; // Gentle, ~6 RPM output

// ULN2003 inputs IN1, IN2, IN3, IN4. Adjacent phases overlap.
const uint8_t HALF_STEP_SEQUENCE[8] = {
  0b0001, 0b0011, 0b0010, 0b0110,
  0b0100, 0b1100, 0b1000, 0b1001
};

SoftwareSerial dfSerial(DFPLAYER_RX_PIN, DFPLAYER_TX_PIN);
DFRobotDFPlayerMini player;

enum State {
  READY,
  THREEPIO_STARTING,
  THREEPIO_PLAYING,
  R2_STARTING,
  R2_PLAYING
};

State state = READY;
unsigned long stateStartedAt = 0;
unsigned long threepioReactionDelayMs = 0;
bool threepioGestureStarted = false;

bool lastButtonReading = HIGH;
bool stableButtonState = HIGH;
unsigned long buttonChangedAt = 0;

// Dome position is always relative to the startup/home position.
long domePosition = 0;
long domeTarget = 0;
uint8_t phaseIndex = 0;
unsigned long lastStepAtUs = 0;
bool coilsEnergized = false;
int8_t firstTurnDirection = 1;

long degreesToSteps(int degrees) {
  return (long)degrees * HALF_STEPS_PER_REV / 360L;
}

void applyMotorPhase(uint8_t phase) {
  uint8_t pattern = HALF_STEP_SEQUENCE[phase];
  for (uint8_t i = 0; i < 4; ++i) {
    digitalWrite(MOTOR_PINS[i], (pattern & (1 << i)) ? HIGH : LOW);
  }
  coilsEnergized = true;
}

void releaseMotor() {
  for (uint8_t i = 0; i < 4; ++i) {
    digitalWrite(MOTOR_PINS[i], LOW);
  }
  coilsEnergized = false;
}

// Called every loop, even during audio playback. Never waits for the motor.
void updateDome() {
  if (domePosition == domeTarget) {
    if (coilsEnergized) releaseMotor(); // No holding torque while stopped.
    return;
  }

  unsigned long nowUs = micros();
  if ((unsigned long)(nowUs - lastStepAtUs) < STEP_INTERVAL_US) return;
  lastStepAtUs = nowUs;

  if (domeTarget > domePosition) {
    phaseIndex = (phaseIndex + 1) & 7;
    ++domePosition;
  } else {
    phaseIndex = (phaseIndex + 7) & 7;
    --domePosition;
  }
  applyMotorPhase(phaseIndex);
}

void aimDome(int degrees) {
  domeTarget = degreesToSteps(degrees);
  Serial.print(F("Dome target (degrees): "));
  Serial.println(degrees);
}

void beginThreepioGesture() {
  firstTurnDirection = (random(2) == 0) ? -1 : 1;
  int angle = random(15, 31); // 15 to 30 degrees
  aimDome(firstTurnDirection * angle);
}

void beginR2Gesture() {
  // Opposite side of home, 15 to 25 degrees.
  // Example: +30, then -20, then home = 30 right, 50 left, 20 right.
  int angle = random(15, 26);
  aimDome(-firstTurnDirection * angle);
}

void enterState(State next) {
  state = next;
  stateStartedAt = millis();
}

void playRandomTrack(uint8_t first, uint8_t last) {
  uint8_t track = random(first, last + 1);
  Serial.print(F("Playing track "));
  Serial.println(track);
  Serial.print(F("Command sent at millis: "));
  Serial.println(millis());
  player.playMp3Folder(track);
}

void setup() {
  pinMode(BUSY_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  for (uint8_t i = 0; i < 4; ++i) {
    pinMode(MOTOR_PINS[i], OUTPUT);
    digitalWrite(MOTOR_PINS[i], LOW);
  }

  Serial.begin(9600);
  dfSerial.begin(9600);
  Serial.println(F("Droids in the Desert - Audio + Animation"));

  delay(1500); // DFPlayer startup, as in original working sketch.

  if (!player.begin(dfSerial)) {
    Serial.println(F("DFPlayer initialization failed."));
    while (true) { }
  }

  player.volume(VOLUME);
  randomSeed(analogRead(A0));
  Serial.println(F("Ready. Press the button!"));
}

void loop() {
  unsigned long now = millis();

  // Always service animation, regardless of audio state.
  updateDome();

  bool reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonReading) {
    buttonChangedAt = now;
  }

  if ((now - buttonChangedAt) >= DEBOUNCE_MS) {
    if (reading != stableButtonState) {
      stableButtonState = reading;
      if (stableButtonState == LOW && state == READY && domePosition == 0) {
        Serial.println(F("Threepio has something to say..."));
        threepioGestureStarted = false;
        playRandomTrack(C3PO_FIRST, C3PO_LAST);
        enterState(THREEPIO_STARTING);
      }
    }
  }
  lastButtonReading = reading;

  bool playing = (digitalRead(BUSY_PIN) == LOW);

  static int lastBusy = -1;
  int currentBusy = digitalRead(BUSY_PIN);
  if (currentBusy != lastBusy) {
    Serial.print(F("BUSY changed at millis: "));
    Serial.print(millis());
    Serial.print(F(" -> "));
    Serial.println(currentBusy == LOW ? F("PLAYING") : F("IDLE"));
    lastBusy = currentBusy;
  }

  switch (state) {
    case READY:
      break;

    case THREEPIO_STARTING:
      if (playing) {
        enterState(THREEPIO_PLAYING);
        threepioReactionDelayMs = random(300, 601);
        Serial.print(F("R2 reaction delay (ms): "));
        Serial.println(threepioReactionDelayMs);
      } else if (millis() - stateStartedAt > START_TIMEOUT_MS) {
        Serial.println(F("Threepio playback did not start."));
        aimDome(0);
        enterState(READY);
      }
      break;

    case THREEPIO_PLAYING:
      if (!threepioGestureStarted &&
          now - stateStartedAt >= threepioReactionDelayMs) {
        beginThreepioGesture();
        threepioGestureStarted = true;
      }
      if (!playing) {
        Serial.println(F("R2's turn!"));
        beginR2Gesture();
        playRandomTrack(R2_FIRST, R2_LAST);
        enterState(R2_STARTING);
      }
      break;

    case R2_STARTING:
      if (playing) {
        enterState(R2_PLAYING);
      } else if (millis() - stateStartedAt > START_TIMEOUT_MS) {
        Serial.println(F("R2 playback did not start."));
        aimDome(0);
        enterState(READY);
      }
      break;

    case R2_PLAYING:
      if (!playing) {
        Serial.println(F("Exchange complete. Returning home."));
        aimDome(0);
        enterState(READY);
      }
      break;
  }
}
