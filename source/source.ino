// Circuit Connection
// Green led = digital 8
// Red led = digital 9
// buzzer = digital 10
// Gas = Analog0
// Ultrasonic triger = digital 11
// Ultrasonic eco = digital 12
// Connect all ground to ground
// Connect all 5v/Vin/Vcc to 5v

#include <NewPing.h>
#include <LiquidCrystal_I2C.h>
#define greenLedPin 8
#define redLedPin 9
#define triggerPin 11
#define echoPin 12
#define maxDistance 50
#define gasPin A0
NewPing sonar(triggerPin, echoPin, maxDistance);
LiquidCrystal_I2C lcd(0x27, 16, 2);
int smellReading = 0, distanceThreshold, gasThreshold;

void setup() {
  Serial.begin(9600);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(gasPin, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibrating...");
  Serial.println("Calibrating...");
  delay(2000);
  distanceThreshold = (sonar.ping() / US_ROUNDTRIP_CM) - 2;
  gasThreshold = analogRead(gasPin) + 5;
  lcd.clear();
  lcd.print("Calibrated...");
  Serial.println("Calibrated...");
  delay(500);
  lcd.clear();
}

void loop() {
  delay(50);
  if ((sonar.ping() / US_ROUNDTRIP_CM) < distanceThreshold) {
    foodAnalyze();
  } else {
    noFood();
  }
}

void foodAnalyze() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Food Detected ");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("  Analyzing... ");
  Serial.println("Food Detected, Analyzing...");
  delay(10000);
  smellReading = analogRead(gasPin);
  if (smellReading < gasThreshold) {
    goodFood();
    foodMonitor();
  } else {
    badFood();
    foodMonitor();
  }
}

void goodFood() {
  digitalWrite(greenLedPin, HIGH);
  digitalWrite(redLedPin, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Methane Rate:" + String(smellReading));
  lcd.setCursor(0, 1);
  lcd.print("  Food is Good  ");
  Serial.println("Methane Rate:" + String(smellReading) + ", Food is Good");
}

void badFood() {
  digitalWrite(greenLedPin, LOW);
  digitalWrite(redLedPin, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Methane Rate:" + String(smellReading));
  lcd.setCursor(0, 1);
  lcd.print(" Food  Spoiled ");
  Serial.println("Methane Rate:" + String(smellReading) + ", Food is Spoiled");
}

void noFood() {
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   Box Empty   ");
  lcd.setCursor(0, 1);
  lcd.print("  Insert Food  ");
  Serial.println("Box Empty, Insert Food");
  delay(500);
  digitalWrite(greenLedPin, HIGH);
  delay(500);
}

void foodMonitor() {
  while ((sonar.ping() / US_ROUNDTRIP_CM) <= distanceThreshold) {
    delay(1000);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Food Removed  ");
  lcd.setCursor(0, 1);
  lcd.print(" Refreshing... ");
  Serial.println("Food Removed, Refreshing...");
  delay(5000);
}