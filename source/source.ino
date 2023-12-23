// Circuit Connection
// Green Led Digital 8
// Red Led Digital 9
// Buzzer digital 10
// Gass sensor Analog0
// Connect all ground to ground
// Connect all 5v/Vin/Vcc to 5v
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
int pinGreenLed = 8;
int pinRedLed = 9;
int buzzer = 10;
int pinSensor = A0;
int THRESHOLD = 37;
int rdata = 0;
void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(pinRedLed, OUTPUT);
  pinMode(pinGreenLed, OUTPUT);
  pinMode(pinSensor, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("   WELCOME    ");
  delay(1000);
  lcd.clear();
  delay(500);
}

void loop() {
  int rdata = analogRead(pinSensor);
  Serial.print("Methane Range: ");
  Serial.println(rdata);
  lcd.setCursor(0, 0);
  lcd.print("Methane: %d", rdata);
  if (rdata >= THRESHOLD) {
    digitalWrite(pinRedLed, HIGH);
    digitalWrite(pinGreenLed, LOW);
    digitalWrite(buzzer, HIGH);
    Serial.print("Food Spoiled");
    lcd.setCursor(0, 1);
    lcd.print("  Food Spoiled  ");
    delay(50);
  } else {
    digitalWrite(pinRedLed, LOW);
    digitalWrite(pinGreenLed, HIGH);
    digitalWrite(buzzer, LOW);
    lcd.setCursor(0, 1);
    lcd.print("   Smell Good   ");
    Serial.print("Food is Good");
  }
  delay(1000);
}
