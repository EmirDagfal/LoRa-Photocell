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
  last = current;
  current = lightIn.read();
  setState();

  return current;
}

bool LightSensor::getState()
{
  setState();
  return state;
}

void LightSensor::setTHold(float high, float low)
{
  highTHold = high;
  lowTHold = low;
}

void LightSensor::setState()
{
  if (last < current)
  {
    // Light is rising
    state = (current > highTHold) ? true : state;
  }
  else
  {
    // Light is descending
    state = (current < lowTHold) ? false : state;
  }
}
