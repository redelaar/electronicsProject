
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED instellingen
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

// Onze rep-counter
int reps = 0;

void setup() {
  Serial.begin(9600);

  // Start het OLED-scherm
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED initialisatie mislukt!");
    while (true);
  }

  display.setTextColor(SSD1306_WHITE);

  // Welkomstscherm
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10, 10);
  display.println("HEY");
  display.setCursor(10, 32);
  display.println("STERREKER");
  display.display();

  delay(2000);
}

void loop() {
  // Maak het scherm leeg
  display.clearDisplay();

  // Titel
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("STERRE'S GYM");

  // Scheidingslijn
  display.drawLine(0, 12, 127, 12, SSD1306_WHITE);

  // Label
  display.setCursor(0, 20);
  display.println("REPS COMPLETED:");

  // Teller in grote letters
  display.setTextSize(3);
  display.setCursor(0, 35);
  display.println(reps);

  // Toon alles op het OLED-scherm
  display.display();

  // Wacht 2 seconden
  delay(2000);

  // Verhoog teller
  reps++;
}
