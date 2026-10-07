#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial dfSerial(10, 11);  // Nano RX, Nano TX
DFRobotDFPlayerMini player;

void setup()
{
  Serial.begin(9600);
  dfSerial.begin(9600);

  Serial.println("DioramaOS DFPlayer test");

  delay(5000);
  pinMode(12, INPUT);

  if (!player.begin(dfSerial)) {
    Serial.println("DFPlayer initialization FAILED.");
    Serial.println("Check wiring and SD card.");
    while (true) {
      delay(1000);
    }
  }

  Serial.println("DFPlayer initialized.");

  player.volume(25);
delay(1000);

Serial.println("Playing track 1...");
player.play(1);
}

void loop()
{
  static int lastBusy = HIGH;
  int busy = digitalRead(12);

  if (busy != lastBusy) {
    Serial.print("DFPlayer BUSY: ");
    Serial.println(busy == LOW ? "PLAYING" : "IDLE");
    lastBusy = busy;
  }
}