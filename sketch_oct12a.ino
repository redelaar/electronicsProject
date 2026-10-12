
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ===== INSTELLINGEN =====

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C

#define RESET_BUTTON 2  // Knop op D2

// ===== OLED =====

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

// ===== VARIABELEN =====

int reps = 0;

// Tijdstip van laatste automatische rep
unsigned long lastRepTime = 0;

// Vorige knopstatus
bool previousButtonState = HIGH;

// ===== SETUP =====

void setup() {
  Serial.begin(9600);

  // Knop instellen met interne pull-up
  pinMode(RESET_BUTTON, INPUT_PULLUP);

  // OLED starten
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED niet gevonden!");
    while (true);
  }

  display.setTextColor(SSD1306_WHITE);

  // Welkomstscherm
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10, 10);
  display.println("HALLO");
  display.setCursor(10, 32);
  display.println("STERRE!");
  display.display();

  delay(2000);

  lastRepTime = millis();
}

// ===== LOOP =====

void loop() {

  // 1. Lees de fysieke resetknop
  bool buttonState = digitalRead(RESET_BUTTON);

  // Detecteer het moment van indrukken
  if (previousButtonState == HIGH &&
      buttonState == LOW) {

    reps = 0;
    lastRepTime = millis();

    Serial.println("RESET! Teller terug naar 0");
  }

  previousButtonState = buttonState;

  // 2. Simuleer elke 2 seconden een rep
  if (millis() - lastRepTime >= 2000) {
    reps++;
    lastRepTime = millis();

    Serial.print("Nieuwe X! Totaal: ");
    Serial.println(reps);
  }

  // 3. Update het OLED-scherm
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("SECRETMESSAGE");

  // Horizontale scheidingslijn
  display.drawLine(
    0, 12, 127, 12, SSD1306_WHITE
  );

  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println("SECRETMESSAGE2:");

  // Grote teller
  display.setTextSize(3);
  display.setCursor(0, 35);
  display.println(reps);

  // Toon op OLED
  display.display();

  delay(20);
}
