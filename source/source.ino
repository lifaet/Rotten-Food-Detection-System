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
#define buzzerPin 10
#define triggerPin 11
#define echoPin 12
#define maxDistance 50
#define gasPin A0
NewPing sonar(triggerPin, echoPin, maxDistance);
LiquidCrystal_I2C lcd(0x27, 16, 2);
int smellReading = 0, distanceThreshold, gasThreshold;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(gasPin, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibrating...");
  delay(2000);
  distanceThreshold = (sonar.ping() / US_ROUNDTRIP_CM) - 2;
  gasThreshold = analogRead(gasPin) + 5;
  lcd.clear();
  lcd.print("Calibrated...");
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
  delay(10000);
  smellReading = analogRead(gasPin);
  if (smellReading < gasThreshold) {
    goodFood();
  } else {
    badFood();
  }
}

void goodFood() {
  digitalWrite(greenLedPin, HIGH);
  digitalWrite(redLedPin, LOW);
  digitalWrite(buzzerPin, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Methane Rate:" + String(smellReading));
  lcd.setCursor(0, 1);
  lcd.print("  Food is Good  ");
  foodMonitor();
}

void badFood() {
  digitalWrite(greenLedPin, LOW);
  digitalWrite(redLedPin, HIGH);
  digitalWrite(buzzerPin, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Methane Rate:" + String(smellReading));
  lcd.setCursor(0, 1);
  lcd.print(" Food  Spoiled ");
  foodMonitor();
}

void noFood() {
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
  digitalWrite(buzzerPin, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   Box Empty   ");
  lcd.setCursor(0, 1);
  lcd.print("  Insert Food  ");
}

void foodMonitor() {
  while ((sonar.ping() / US_ROUNDTRIP_CM) < distanceThreshold) {
    delay(1000);
  }
  digitalWrite(buzzerPin, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Food Removed  ");
  lcd.setCursor(0, 1);
  lcd.print(" Refreshing... ");
  delay(5000);
}