/**
 * @file    CurrentSensor.h
 * @brief   Driver para sensor ACS722
 * @author  Emir Dagfal
 * @version 1.0
 *
 * Circuito caracteristico para el sensor de corriente ACS722
 *
 */

#ifndef __ED_CURRENT_SENSOR__
#define __ED_CURRENT_SENSOR__

#include "mbed.h"

// * Constants
const float curr_sens = 1.352; // I/Vpp [mA/mV = A/V = 1/ohm]
const float thr_curr = .15;    // zona muerta del sensor [Ampere]

class CurrentSensor
{
private:
  AnalogIn currentIn;
  Timer t;
  float current;
  float power;
  void setValues();

public:
  CurrentSensor(/* args */);
  ~CurrentSensor();
  float get();
  float getEstimatedPower();
};

CurrentSensor::CurrentSensor(/* args */) : currentIn(PB_12)
{
  current = 0;
  power = 0;
}

CurrentSensor::~CurrentSensor()
{
}

#endif
