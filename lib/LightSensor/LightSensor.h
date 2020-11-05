/**
 * @file    LightSensor.h
 * @brief   Driver para sensor ALS-PT204-6C/L177
 * @author  Emir Dagfal
 * @version 1.0
 *
 * El circuito consta del sensor ALS-PT204-6C/L177 con
 * una resistencia de 10Kohm conectada al emisor
 *
 */

#ifndef __ED_LIGHT_SENSOR__
#define __ED_LIGHT_SENSOR__

#include "index.h"

// * Constants
const float HIGHT = 0.7;
const float LOW = 0.65;

class LightSensor
{
private:
  AnalogIn lightIn;
  bool state;
  float current;
  float last;
  float highTHold;
  float lowTHold;

  void setState();

public:
  LightSensor(/* args */);
  ~LightSensor();
  float get();
  bool getState();
  void setTHold(float high = HIGHT, float low = LOW);
};

LightSensor::LightSensor(/* args */) : lightIn(PB_13)
{
  state = true;
}

LightSensor::~LightSensor()
{
}

#endif
