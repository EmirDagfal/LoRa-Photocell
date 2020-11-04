/**
 * @file    CurrentSensor.h
 * @brief   Driver para sensor ACS722
 * @author  Emir Dagfal
 * @version 1.0
 *
 * Circuito caracteristico para el sensor de corriente ACS722
 *
 */

#include "CurrentSensor.h"

CurrentSensor::get()
{
  this.setValues();
  return this.current; // value in Ampere
}

CurrentSensor::getEstimatedPower()
{
  this.setValues();
  return this.power; //value in W
}

CurrentSensor::setValues()
{
  float max = 0.0;
  float min = 1.0;
  float value = 0;

  timer_.reset();
  timer_.start();
  while (timer_.read_ms() < 40)
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
  timer_.stop();
  value = (max - min) * curr_sens * 3.3;
  if (value < thr_curr)
  {
    value = 0;
  }

  this.current = value;
  this.power = value * 220;
}
