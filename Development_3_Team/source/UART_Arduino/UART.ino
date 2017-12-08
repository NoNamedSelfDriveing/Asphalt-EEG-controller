
void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
   if (Serial1.available())
  {
     Serial.write(Serial1.read());
  }

}
  
/*
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  delay(100);
*/
  /*
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
  */
