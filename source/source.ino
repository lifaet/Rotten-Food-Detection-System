// Circuit Connection
// Green Led Digital 8
// Red Led Digital 9
// Buzzer digital 10
// Gass sensor Analog0
// Connect all ground to ground
// Connect all 5v/Vin/Vcc to 5v

int pinGreenLed = 8;
int pinRedLed = 9;
int buzzer = 10;
int pinSensor = A0;
int THRESHOLD = 250;
int rdata = 0;
void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(pinRedLed, OUTPUT);
  pinMode(pinGreenLed, OUTPUT);
  pinMode(pinSensor, INPUT);
  delay(500);
}

void loop() {
  int rdata = analogRead(pinSensor);
  Serial.print("Methane Range: ");
  Serial.println(rdata);
  if (rdata >= THRESHOLD) {
    digitalWrite(pinRedLed, HIGH);
    digitalWrite(pinGreenLed, LOW);
    digitalWrite(buzzer, HIGH);
    Serial.print("Food Spoiled");
    delay(50);
  } else {
    digitalWrite(pinRedLed, LOW);
    digitalWrite(pinGreenLed, HIGH);
    digitalWrite(buzzer, LOW);
    Serial.print("Food is Good");
  }
  delay(1000);
}
