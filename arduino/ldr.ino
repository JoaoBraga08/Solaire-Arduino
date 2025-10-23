#define ENTRADA_ANALOGICA A0
int valor_adc_LDR = 0;

void setup() {
  Serial.begin(9600); // usar mesma baud rate no Node.js
}

void loop() {
  valor_adc_LDR = analogRead(ENTRADA_ANALOGICA);
  Serial.print("LDR: ");
  Serial.println(valor_adc_LDR);
  delay(500);
}
