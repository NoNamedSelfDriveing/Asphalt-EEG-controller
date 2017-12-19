typedef union _data
{
  uint8_t  buff[4];
  float    value;  
}DATA;

DATA data;
uint8_t receive_data;
uint8_t packet_data[4];
int receive_i = 0, packet_i = 0;
unsigned char start_flag = 0;

void setup(){
  Serial.begin(9600);
}

void loop(){
  if(Serial.available())
  {
   receive_data = Serial.read();
   //Serial.println(receive_data[receive_i], HEX);

   if(receive_data == 0x01)
   {
     start_flag = 0;
   }
   
   if(start_flag == 1)
   {
    packet_data[packet_i] = receive_data;
    Serial.println(packet_data[packet_i], HEX);
    if(++packet_i >= 4)
    {
     packet_i = 0;
     Serial.println("start");
     }
    }

    if(receive_data == 0xfe)
    {
      start_flag = 1;
    }
  }
}
