#include "MPU6050.h"

void setup()
{
  init_mpu();
  init_kalman(0.1, 0.003, 0.03);
}

void loop()
{
  get_mpu_value();

  Serial.print("ROLL : ");
  Serial.print(mpu[ROLL].value);
  Serial.print(" | PITCH : ");
  Serial.print(mpu[PITCH].value);
  Serial.print(" | YAW : ");
  Serial.println(mpu[YAW].value);
}

