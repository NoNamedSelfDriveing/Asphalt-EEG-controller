#ifndef MPU6050_H
#define MPU6050_H

#include "Arduino.h"

#define START_BYTE 0xFE
#define END_BYTE 0x01
#define PACKET_SIZE 14
#define ACCEL_XOUT_H 0x3B
#define ROLL 0
#define PITCH 1
#define YAW 2

typedef union _mpu6050{
  uint8_t buff[4];
  float value;  
}MPU6050;

typedef struct _kalman{
  double P[2][2];
  double K[2];
  double angle;
  double bias;
  double Q_angle;
  double Q_gyro;
  double R_measure;
}KALMAN;

extern MPU6050 mpu[3];
extern KALMAN kalman[3];

void init_kalman(double angle, double gyro, double measure);
float get_kalman(int axis, double acc, double gyro, double dt);
void init_mpu();
void read_MPU(uint8_t start_addr);
void make_mpu_packet();
void get_mpu_value();

#endif
