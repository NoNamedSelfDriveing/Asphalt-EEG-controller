#include "Wire.h"
#include "MPU6050.h"

MPU6050 mpu[3];
KALMAN kalman[3];

uint8_t mpu_packet_buff[PACKET_SIZE];
const int MPU_addr = 0x68;
int16_t acX, acY, acZ, tmp, gyX, gyY, gyZ;
uint32_t past_time;
double delta_T;

void init_kalman(double angle, double gyro, double measure)
{
  for(int i = 0; i < 3; i++)
  {
    kalman[i].Q_angle = angle;
    kalman[i].Q_gyro = gyro;
    kalman[i].R_measure = measure;
  
    kalman[i].angle = 0;
    kalman[i].bias = 0;
  
    kalman[i].P[0][0] = 0;
    kalman[i].P[0][1] = 0;
    kalman[i].P[1][0] = 0;
    kalman[i].P[1][1] = 0;
  }
}

float get_kalman(int axis, double acc, double gyro, double dt)
{
  kalman[axis].angle += dt * (gyro - kalman[axis].bias);

  kalman[axis].P[0][0] += dt * (dt * kalman[axis].P[1][1] - kalman[axis].P[0][1] - kalman[axis].P[1][0] + kalman[axis].Q_angle);
  kalman[axis].P[0][1] -= dt * kalman[axis].P[1][1];
  kalman[axis].P[1][0] -= dt * kalman[axis].P[1][1];
  kalman[axis].P[1][1] += kalman[axis].Q_gyro * dt;

  double S = kalman[axis].P[0][0] + kalman[axis].R_measure;
  kalman[axis].K[0] = kalman[axis].P[0][0] / S;
  kalman[axis].K[1] = kalman[axis].P[1][0] / S;

  double y = acc - kalman[axis].angle;
  kalman[axis].angle += kalman[axis].K[0] * y;
  kalman[axis].bias += kalman[axis].K[1] * y;

  double P_temp[2] = { kalman[axis].P[0][0], kalman[axis].P[0][1] };
  kalman[axis].P[0][0] -= kalman[axis].K[0] * P_temp[0];
  kalman[axis].P[0][1] -= kalman[axis].K[0] * P_temp[1];
  kalman[axis].P[1][0] -= kalman[axis].K[1] * P_temp[0];
  kalman[axis].P[1][1] -= kalman[axis].K[1] * P_temp[1];

  return kalman[axis].angle;
}

void init_mpu()
{
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x1B);
  Wire.write(0x18);
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void read_MPU(uint8_t start_addr)
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(start_addr);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);
  acX = (Wire.read() << 8) | (Wire.read());
  acY = (Wire.read() << 8) | (Wire.read());
  acZ = (Wire.read() << 8) | (Wire.read());
  tmp = (Wire.read() << 8) | (Wire.read());
  gyX = (Wire.read() << 8) | (Wire.read());
  gyY = (Wire.read() << 8) | (Wire.read());
  gyZ = (Wire.read() << 8) | (Wire.read());
}

void make_mpu_packet()
{
  mpu_packet_buff[0] = START_BYTE;
  mpu_packet_buff[PACKET_SIZE - 1] = END_BYTE;

  /* ROLL data */
  mpu_packet_buff[1] = mpu[0].buff[3];
  mpu_packet_buff[2] = mpu[0].buff[2];
  mpu_packet_buff[3] = mpu[0].buff[1];
  mpu_packet_buff[4] = mpu[0].buff[0];

  /* PITCH data */
  mpu_packet_buff[5] = mpu[1].buff[3];
  mpu_packet_buff[6] = mpu[1].buff[2];
  mpu_packet_buff[7] = mpu[1].buff[1];
  mpu_packet_buff[8] = mpu[1].buff[0];

  /* YAW data */
  mpu_packet_buff[9] = mpu[2].buff[3];
  mpu_packet_buff[10] = mpu[2].buff[2];
  mpu_packet_buff[11] = mpu[2].buff[1];
  mpu_packet_buff[12] = mpu[2].buff[0];
}

void get_mpu_value()
{
  read_MPU(ACCEL_XOUT_H);
  float FS_SEL = 16.4;
  float gyro_x = gyX/FS_SEL;
  float gyro_y = gyY/FS_SEL;
  float gyro_z = gyZ/FS_SEL;

  float accel_x = acX;
  float accel_y = acY;
  float accel_z = acZ;

  float RADIANS_TO_DEGRESS = 180/3.141592653589793;
  float accel_angle_x = atan(-1*accel_x/sqrt(pow(accel_y, 2)+pow(accel_z, 2))) * RADIANS_TO_DEGRESS;
  float accel_angle_y = atan(-1*accel_y/sqrt(pow(accel_x, 2)+pow(accel_z, 2))) * RADIANS_TO_DEGRESS;
  float accel_angle_z = 0;

  delta_T = (double)(micros() - past_time) / 1000000;
  past_time = micros();

  mpu[ROLL].value = get_kalman(ROLL, accel_angle_x, gyro_x, delta_T);
  mpu[PITCH].value = get_kalman(PITCH, accel_angle_y, gyro_y, delta_T);
  mpu[YAW].value = get_kalman(YAW, accel_angle_z, gyro_z, delta_T);

  //Serial.println(mpu[ROLL].value);
}



