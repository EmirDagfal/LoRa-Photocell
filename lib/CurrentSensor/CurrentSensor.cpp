/**
 * @file    CurrentSensor.cpp
 * @brief   Driver para sensor ACS722
 * @author  Emir Dagfal
 * @version 1.0
 *
 * Circuito caracteristico para el sensor de corriente ACS722
 *
 */

#include "CurrentSensor.h"

float CurrentSensor::get()
{
  setValues();
  return current; // value in Ampere
}

float CurrentSensor::getEstimatedPower()
{
  setValues();
  return power; //value in W
}

void CurrentSensor::setValues()
{
  float max = 0.0;
  float min = 1.0;
  float value = 0;

  t.reset();
  t.start();
  while (t.read_ms() < 40)
  {
    value = ((currentIn.read()) - 0.5);
    if (value < min)
    {
      min = value;
    }
    if (value > max)
    {
      max = value;
    }
  }
  t.stop();
  value = (max - min) * curr_sens * 3.3;
  if (value < thr_curr)
  {
    value = 0;
  }

  current = value;
  power = value * 220;
}
