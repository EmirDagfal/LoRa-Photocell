/**
 * @file    LightSensor.cpp
 * @brief   Driver para sensor ALS-PT204-6C/L177
 * @author  Emir Dagfal
 * @version 1.0
 *
 * El circuito consta del sensor ALS-PT204-6C/L177 con
 * una resistencia de 10Kohm conectada al emisor
 *
 */
#include "LightSensor.h"

float LightSensor::get()
{
  this.last = this.current;
  this.current = lightIn.read();
  this.setState();

  return this.current;
}

bool LightSensor::getState()
{
  this.setState();
  return this.state;
}

LightSensor::setTHold(float high = HIGHT, float low = LOW)
{
  this.highTHold = high;
  this.lowTHold = low;
}

LightSensor::setState()
{
  if (this.last < this.current)
  {
    // Light is rising
    this.state = (this.current > this.highTHold) ? true : this.state;
  }
  else
  {
    // Light is descending
    this.state = (this.current < this.lowThold) ? false : this.state;
  }
}
