typedef union 
{
  uint8_t  buff[4];
  float    value;  
}DATA;

DATA data;
uint8_t send_data[6];
int i;

void setup(){
  Serial.begin(9600);
}

void loop(){
  data.value = 3.141592;
  
  send_data[0] = 0xfe;
  for(i=1; i<5; i++) 
    send_data[i] = data.buff[i-1];  
  send_data[5] = 0x01;
 
  for(i=0; i<6; i++)
    Serial.write(send_data[i]);
  delay(500);
}
