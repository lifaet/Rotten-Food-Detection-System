// Circuit Connection
// Green Led digital 8
// Red Led digital 9
// Buzzer digital 10
// Gass sensor Analog0
// Ultrasonic triger 11
// Ultrasonic eco 12
// Connect all ground to ground
// Connect all 5v/Vin/Vcc to 5v

#include <NewPing.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

#define TRIGGER_PIN 11
#define ECHO_PIN 12
#define MAX_DISTANCE 200

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int pinGreenLed = 8;
int pinRedLed = 9;
int buzzer = 10;
int pinSensor = A0;
int rdata = 0;
int DETECTION_DISTANCE;
int THRESHOLD;

void setup()
{
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(pinRedLed, OUTPUT);
  pinMode(pinGreenLed, OUTPUT);
  pinMode(pinSensor, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibrating...");
  Serial.println("Calibrating...");
  DETECTION_DISTANCE = sonar.ping() / US_ROUNDTRIP_CM - 2;
  THRESHOLD = analogRead(pinSensor) + 5;
  delay(1500);
  lcd.clear();
  lcd.print("Calibrated...");
  Serial.println("Calibrated...");
  delay(500);
  lcd.clear();
}

void loop()
{
  delay(50);
  unsigned int uS = sonar.ping();
  if (uS / US_ROUNDTRIP_CM < DETECTION_DISTANCE)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Food Detected ");
    Serial.println("Food Detected");
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("  Analyzing... ");
    Serial.println("Analyzing...");
    delay(10000);
    int rdata = analogRead(pinSensor);
    lcd.clear();
    if (rdata < THRESHOLD)
    {
      digitalWrite(pinRedLed, LOW);
      digitalWrite(pinGreenLed, HIGH);
      digitalWrite(buzzer, LOW);
      lcd.setCursor(0, 0);
      lcd.print("Methane Rate:" + String(rdata));
      lcd.setCursor(0, 1);
      lcd.print("  Food is Good  ");
      Serial.println("Food is Good...");
      delay(5000);
      exit(0);
    }
    else
    {
      digitalWrite(pinRedLed, HIGH);
      digitalWrite(pinGreenLed, LOW);
      digitalWrite(buzzer, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("Methane Rate:" + String(rdata));
      lcd.setCursor(0, 1);
      lcd.print(" Food  Spoiled ");
      Serial.println("Food Spoiled");
      delay(5000);
      exit(0);
    }
  }
  else
  {
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Box Empty   ");
    Serial.println("Box Empty");
    lcd.setCursor(0, 1);
    lcd.print("  Insert Food  ");
  }
}