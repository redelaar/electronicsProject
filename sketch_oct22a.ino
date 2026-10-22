#include <Wire.h>

const byte MPU = 0x68;

// Drempels op basis van jouw metingen
const float TOP_Z_THRESHOLD = -5000;
const float TOP_Y_THRESHOLD = 1500;
const float BOTTOM_Z_THRESHOLD = 7000;

const unsigned long MIN_REP_TIME = 400;

float filteredY = 0;
float filteredZ = 0;
bool filterGestart = false;

bool onderBereikt = false;
bool bovenBereikt = false;

int reps = 0;

unsigned long laatsteRepTijd = 0;
unsigned long laatstePrintTijd = 0;

byte schrijfRegister(byte registerAdres, byte waarde) {
  Wire.beginTransmission(MPU);
  Wire.write(registerAdres);
  Wire.write(waarde);
  return Wire.endTransmission();
}

bool leesAcceleratie(int16_t &x, int16_t &y, int16_t &z) {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);

  if (Wire.endTransmission(false) != 0) {
    return false;
  }

  if (Wire.requestFrom(MPU, (byte)6) != 6) {
    return false;
  }

  x = ((int16_t)Wire.read() << 8) | Wire.read();
  y = ((int16_t)Wire.read() << 8) | Wire.read();
  z = ((int16_t)Wire.read() << 8) | Wire.read();

  return true;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(100000);

  delay(500);

  // Sensor resetten
  schrijfRegister(0x6B, 0x80);
  delay(200);

  // Sensor wakker maken
  byte fout = schrijfRegister(0x6B, 0x01);
  delay(200);

  Serial.print("MPU gestart. Foutcode: ");
  Serial.println(fout);
  Serial.println("Begin met de halter in de onderste positie.");
}

void loop() {
  int16_t x, y, z;

  if (!leesAcceleratie(x, y, z)) {
    Serial.println("Communicatiefout met MPU");
    delay(100);
    return;
  }

  // Filter starten met de eerste echte meting
  if (!filterGestart) {
    filteredY = y;
    filteredZ = z;
    filterGestart = true;
  }

  // Low-passfilter tegen korte schokken en trillingen
  filteredY = 0.75 * filteredY + 0.25 * y;
  filteredZ = 0.75 * filteredZ + 0.25 * z;

  // Onderste positie herkennen
  if (!bovenBereikt &&
      filteredZ > BOTTOM_Z_THRESHOLD) {
    onderBereikt = true;
  }

  // Bovenste positie herkennen
  if (onderBereikt &&
      !bovenBereikt &&
      filteredZ < TOP_Z_THRESHOLD &&
      filteredY > TOP_Y_THRESHOLD) {

    bovenBereikt = true;
    Serial.println("      ^^^ BOVENSTE POSITIE ^^^");
  }

  // Terug onderaan: één volledige rep
  if (bovenBereikt &&
      filteredZ > BOTTOM_Z_THRESHOLD &&
      millis() - laatsteRepTijd > MIN_REP_TIME) {

    reps++;
    laatsteRepTijd = millis();

    Serial.print("========== REP ");
    Serial.print(reps);
    Serial.println(" GEDETECTEERD! ==========");

    bovenBereikt = false;
    onderBereikt = true;
  }

  // Debuggegevens iedere 100 ms printen
  if (millis() - laatstePrintTijd >= 100) {
    laatstePrintTijd = millis();

    Serial.print("X: ");
    Serial.print(x);

    Serial.print("  Y: ");
    Serial.print(y);

    Serial.print("  Z: ");
    Serial.print(z);

    Serial.print("  | FilterY: ");
    Serial.print((int)filteredY);

    Serial.print("  FilterZ: ");
    Serial.print((int)filteredZ);

    Serial.print("  | Positie: ");

    if (bovenBereikt) {
      Serial.print("BOVEN");
    } else if (onderBereikt) {
      Serial.print("ONDER");
    } else {
      Serial.print("ZOEKEN");
    }

    Serial.print("  | Reps: ");
    Serial.println(reps);
  }

  delay(25);
}
