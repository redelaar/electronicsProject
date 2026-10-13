
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mp3Serial(10, 11);
DFRobotDFPlayerMini mp3;

// PAS DIT AAN naar het aantal MP3's op je SD-kaart
const int AANTAL_GELUIDEN = 30;

const unsigned long INTERVAL = 5000; // 5 seconden
unsigned long laatsteGeluid = 0;

void speelRandomGeluid() {
  int nummer = random(1, AANTAL_GELUIDEN + 1);

  Serial.print("Spelen: /mp3/");
  if (nummer < 10) Serial.print("000");
  else if (nummer < 100) Serial.print("00");
  else if (nummer < 1000) Serial.print("0");
  Serial.print(nummer);
  Serial.println(".mp3");

  mp3.playMp3Folder(nummer);
}

void setup() {
  Serial.begin(9600);
  mp3Serial.begin(9600);

  Serial.println("=== STERRE'S RANDOM SOUND MACHINE ===");

  delay(3000);

  if (!mp3.begin(mp3Serial, false, false)) {
    Serial.println("WAARSCHUWING: Geen antwoord van DFPlayer");
  } else {
    Serial.println("DFPlayer initialisatie OK!");
  }

  mp3.volume(24);

  // Zorgt dat de gekozen nummers niet steeds
  // in dezelfde volgorde verschijnen.
  randomSeed(analogRead(A0));

  Serial.println("Random geluiden starten!");
  delay(500);

  speelRandomGeluid();
  laatsteGeluid = millis();
}

void loop() {
  if (millis() - laatsteGeluid >= INTERVAL) {
    laatsteGeluid = millis();
    speelRandomGeluid();
  }
}
