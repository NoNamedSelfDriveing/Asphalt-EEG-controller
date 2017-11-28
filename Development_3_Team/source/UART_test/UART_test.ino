/*
int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
char ch = 0;
*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    Serial.write(Serial.read());
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
}
