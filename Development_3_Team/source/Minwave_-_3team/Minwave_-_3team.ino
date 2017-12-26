#include "MPU6050.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <MsTimer2.h>
#include <TimerOne.h>

#define DELAY 4
#define TIME 1
#define LEFT_MAX 30
#define RIGHT_MAX -30
#define BACK_MIN -25

void check_mpu_value();
void wait_release();

int press_delay = 0;
int release_flag = 0;
char key_check = '0';

void setup()
{
  init_mpu();
  init_kalman(0.1, 0.003, 0.03);

  MsTimer2::set(TIME, check_mpu_value); //ddd
  MsTimer2::start();
}

void loop()
{
  get_mpu_value();

  Serial.print("kalman degree ");
  Serial.print("ROLL : ");
  Serial.print(mpu[ROLL].value);
  Serial.print(" | PITCH : ");
  Serial.print(mpu[PITCH].value);
  Serial.print(" | YAW : ");
  Serial.println(mpu[YAW].value);
  Serial.println("");

}

void wait_release()
{
  Keyboard.release(key_check);

  release_flag = 0;
  key_check = '0';
}

void check_mpu_value()
{
  int value = 0;

  if (release_flag == 0)
  {
    if (mpu[PITCH].value < LEFT_MAX && mpu[PITCH].value > RIGHT_MAX)
    {
      //Keyboard.write('w');
    }
    else if (mpu[PITCH].value <= RIGHT_MAX)
    {
      Keyboard.press('d');

      release_flag = 1;
      key_check = 'd';
      press_delay = DELAY;

      Timer1.initialize(press_delay * 1000); // 100000 = 0.1s = 100ms
      Timer1.attachInterrupt(wait_release); // attach the service routine here

    }
    else if (mpu[PITCH].value >= LEFT_MAX)
    {
      Keyboard.press('a');

      release_flag = 1;
      key_check = 'a';
      press_delay = DELAY;

      Timer1.initialize(press_delay * 1000); // 100000 = 0.1s = 100ms
      Timer1.attachInterrupt(wait_release); // attach the service routine here
    }
    if (mpu[ROLL].value <= BACK_MIN)
    {
      Keyboard.press('s');

      release_flag = 1;
      key_check = 's';
      press_delay = DELAY;

      Timer1.initialize(press_delay * 1000); // 100000 = 0.1s = 100ms
      Timer1.attachInterrupt(wait_release); // attach the service routine here
    }
    //Serial.println("");
  }
}
