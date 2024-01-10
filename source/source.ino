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
#define maxDistance 200
#define gasPin A0
NewPing sonar(triggerPin, echoPin, maxDistance);
LiquidCrystal_I2C lcd(0x27, 16, 2);
int smellReading = 0, distanceThreshold, gasThreshold;

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
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
  distanceThreshold = sonar.ping() / US_ROUNDTRIP_CM - 2;
  gasThreshold = analogRead(gasPin) + 5;
  lcd.clear();
  lcd.print("Calibrated...");
  Serial.println("Calibrated...");
  delay(500);
  lcd.clear();
}

void loop() {
  delay(50);
  if (sonar.ping() / US_ROUNDTRIP_CM < distanceThreshold) {
    foodAnalyze();
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Box Empty   ");
    lcd.setCursor(0, 1);
    lcd.print("  Insert Food  ");
    Serial.println("Box Empty, Insert Food");
    delay(500);
  }
}

void foodAnalyze() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Food Detected ");
  Serial.println("Food Detected");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("  Analyzing... ");
  Serial.println("Analyzing...");
  delay(10000);
  lcd.clear();
  smellReading = analogRead(gasPin);
  if (smellReading < gasThreshold) {
    goodFood();
    foodMoitor();
  } else {
    badFood();
    foodMoitor();
  }
}

void goodFood() {
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, HIGH);
  digitalWrite(buzzerPin, LOW);
  lcd.setCursor(0, 0);
  lcd.print("Methane Rate:" + String(smellReading));
  lcd.setCursor(0, 1);
  lcd.print("  Food is Good  ");
  Serial.println("Food is Good");
}

void badFood() {
  digitalWrite(redLedPin, HIGH);
  digitalWrite(greenLedPin, LOW);
  digitalWrite(buzzerPin, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Methane Rate:" + String(smellReading));
  lcd.setCursor(0, 1);
  lcd.print(" Food  Spoiled ");
  Serial.println("Food Spoiled");
}

void foodMoitor() {
  while (sonar.ping() / US_ROUNDTRIP_CM < distanceThreshold) {
    delay(1000);
  }
}