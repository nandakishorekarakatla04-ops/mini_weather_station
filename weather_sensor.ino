const int rainSensor = A3;
const int led = 4;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  int value = analogRead(rainSensor);

  Serial.print("Value : ");
  Serial.print(value);

  if (value < 200) {
    digitalWrite(led, HIGH);
    Serial.println("  | HEAVY RAIN - LED ON");
  }
  else if (value < 250) {
    digitalWrite(led, HIGH);
    Serial.println("  | RAIN - LED ON");
  }
  else {
    digitalWrite(led, LOW);
    Serial.println("  | NO RAIN - LED OFF");
  }

  delay(1000);
}