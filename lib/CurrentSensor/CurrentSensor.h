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

#include "index.h"

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
  setValues();

public:
  CurrentSensor(/* args */);
  ~CurrentSensor();
  get();
  getEstimatedPower();
};

CurrentSensor::CurrentSensor(/* args */)
{
  this.currentIn(PB_12);
  this.current = 0;
  this.power = 0;
}

CurrentSensor::~CurrentSensor()
{
}

#endif
