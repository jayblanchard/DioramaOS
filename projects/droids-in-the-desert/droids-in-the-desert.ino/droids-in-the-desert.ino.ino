
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// Hardware connections
constexpr uint8_t DFPLAYER_RX_PIN = 10; // Nano receives from DFPlayer TX
constexpr uint8_t DFPLAYER_TX_PIN = 11; // Nano transmits to DFPlayer RX via 1k resistor
constexpr uint8_t BUSY_PIN       = 12; // LOW while playing
constexpr uint8_t BUTTON_PIN     = 7;  // Button to GND

// Audio tracks in the SD card's /mp3 directory
constexpr uint8_t R2_FIRST   = 1;
constexpr uint8_t R2_LAST    = 10;
constexpr uint8_t C3PO_FIRST = 11;
constexpr uint8_t C3PO_LAST  = 20;

constexpr uint8_t VOLUME = 22; // DFPlayer range: 0-30
constexpr unsigned long DEBOUNCE_MS = 40;
constexpr unsigned long START_TIMEOUT_MS = 5000;

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

// Button debounce
bool lastButtonReading = HIGH;
bool stableButtonState = HIGH;
unsigned long buttonChangedAt = 0;

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

  Serial.begin(9600);
  dfSerial.begin(9600);

  Serial.println(F("Droids in the Desert - Initializing "));

  // Give the DFPlayer time to finish powering up.
  delay(1500);

  if (!player.begin(dfSerial)) {
    Serial.println(F("DFPlayer initialization failed."));
    while (true) {
      // Stop here until wiring or SD card is corrected.
    }
  }

  player.volume(VOLUME);
  randomSeed(analogRead(A0));

  Serial.println(F("Ready. Press the button!"));
}

void loop() {
  unsigned long now = millis();

  // Debounce the momentary button.
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    buttonChangedAt = now;
  }

  if ((now - buttonChangedAt) >= DEBOUNCE_MS) {
    if (reading != stableButtonState) {
      stableButtonState = reading;

      // Only respond to a new press while idle.
      if (stableButtonState == LOW && state == READY) {
        Serial.println(F("Threepio has something to say..."));
        playRandomTrack(C3PO_FIRST, C3PO_LAST);
        enterState(THREEPIO_STARTING);
      }
    }
  }

  lastButtonReading = reading;

  // LOW = audio playing, HIGH = idle.
  bool playing = (digitalRead(BUSY_PIN) == LOW);

  // Diagnostic: report changes in the DFPlayer BUSY signal.
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
      // Wait until the DFPlayer actually begins playback.
      if (playing) {
        enterState(THREEPIO_PLAYING);
      } else if (millis() - stateStartedAt > START_TIMEOUT_MS) {
        Serial.println(F("Threepio playback did not start."));
        enterState(READY);
      }
      break;

    case THREEPIO_PLAYING:
      if (!playing) {
        Serial.println(F("R2's turn!"));
        playRandomTrack(R2_FIRST, R2_LAST);
        enterState(R2_STARTING);
      }
      break;

    case R2_STARTING:
      if (playing) {
        enterState(R2_PLAYING);
      } else if (millis() - stateStartedAt > START_TIMEOUT_MS) {
        Serial.println(F("R2 playback did not start."));
        enterState(READY);
      }
      break;

    case R2_PLAYING:
      if (!playing) {
        Serial.println(F("Exchange complete. Ready."));
        enterState(READY);
      }
      break;
  }
}
