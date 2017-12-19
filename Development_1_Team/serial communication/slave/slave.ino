#define START_BYTE 0xFE
#define END_BYTE 0x01
#define PACKET_SIZE 12

typedef union _data
{
  uint8_t  buff[PACKET_SIZE];
  float    value[3];  
}MPUData;

MPUData mpu_data;
uint8_t receive_data;
uint8_t packet_data[PACKET_SIZE];
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

   if(receive_data == END_BYTE)
   {
     start_flag = 0;
   }
   
   if(start_flag == 1)
   {
    //packet_data[packet_i] = receive_data;
    mpu_data.buff[packet_i] = receive_data;
    //Serial.println(packet_data[packet_i], HEX);
    if(++packet_i >= PACKET_SIZE)
    {
     packet_i = 0;
     Serial.println("start");
     Serial.println(mpu_data.value[0]);
     Serial.println(mpu_data.value[1]);
     Serial.println(mpu_data.value[2]);
     }
    }

    if(receive_data == START_BYTE)
    {
      start_flag = 1;
    }
  }
}
