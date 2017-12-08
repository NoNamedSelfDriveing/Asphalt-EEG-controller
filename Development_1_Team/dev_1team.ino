#include <Wire.h>

#define ACCEL_XOUT_H 0x3B

const int MPU = 0x68;
int16_t acX, acY, acZ, tmp, gyX, gyY, gyZ;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);
  Wire.write(0x18);
  Wire.endTransmission(true);
  Serial.begin(9600);
}

long get_dt(long curr_time)
{
  static long prev_time = 0;
  static long dt = 0;

  dt = curr_time - prev_time;
  prev_time = curr_time;

  return dt;
}

float last_gyro_angle_x = 0.0;
float last_gyro_angle_y = 0.0;
float last_gyro_angle_z = 0.0;

void set_last_gyro_angles(float gyro_angle_x, float gyro_angle_y, float gyro_angle_z)
{
  last_gyro_angle_x = gyro_angle_x;
  last_gyro_angle_y = gyro_angle_y;
  last_gyro_angle_z = gyro_angle_z;
}

void loop() {
  long t_now = millis();
  // put your main code here, to run repeatedly:
  read_MPU(ACCEL_XOUT_H);
  float FS_SEL = 16.4;
  float gyro_x = gyX/FS_SEL;
  float gyro_y = gyY/FS_SEL;
  float gyro_z = gyZ/FS_SEL;

  float accel_x = acX;
  float accel_y = acY;
  float accel_z = acZ;

  float RADIANS_TO_DEGRESS = 180/3.14159;
  float accel_angle_x = atan(-1*accel_x/sqrt(pow(accel_y, 2)+pow(accel_z, 2))) * RADIANS_TO_DEGRESS;
  float accel_angle_y = atan(-1*accel_y/sqrt(pow(accel_x, 2)+pow(accel_z, 2))) * RADIANS_TO_DEGRESS;
  float accel_angle_z = 0;
  
  float dt = get_dt(t_now)/1000.0;
  float gyro_angle_x = gyro_x * dt + last_gyro_angle_x;
  float gyro_angle_y = gyro_y * dt + last_gyro_angle_y;
  float gyro_angle_z = gyro_z * dt + last_gyro_angle_z;
  set_last_gyro_angles(gyro_angle_x, gyro_angle_y, gyro_angle_z);

  float alpha = 0.96;
  float angle_x = gyro_angle_x;//0.99 * gyro_angle_x + 0.02 * accel_angle_x;
  float angle_y = alpha * gyro_angle_y + (1-alpha) * accel_angle_y;
  float angle_z = gyro_angle_z;
  
  Serial.print("angle_x = "); Serial.print(angle_x);
  Serial.print(" | angle_y = "); Serial.print(angle_y);
  Serial.print(" | angle_z = "); Serial.println(angle_z);
  //Serial.print("x= "); Serial.print(accel_angle_x);
  //Serial.print(" | y= "); Serial.print(accel_angle_y);
  //Serial.print(" | z= "); Serial.println(accel_angle_z);
  //delay(300);
  //delay(5);
}

void read_MPU(uint8_t start_addr)
{
  Wire.beginTransmission(MPU);
  Wire.write(start_addr);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);
  acX = (Wire.read() << 8) | (Wire.read());
  acY = (Wire.read() << 8) | (Wire.read());
  acZ = (Wire.read() << 8) | (Wire.read());
  tmp = (Wire.read() << 8) | (Wire.read());
  gyX = (Wire.read() << 8) | (Wire.read());
  gyY = (Wire.read() << 8) | (Wire.read());
  gyZ = (Wire.read() << 8) | (Wire.read());
}
