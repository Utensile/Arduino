double variable = 0.00;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(variable);
  variable = variable + 0.01;
  delay(100);
}