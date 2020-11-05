/**
 * @file    LuminaryDriver.h
 * @brief   Control 0-10v para Luminarias
 * @author  Emir Dagfal
 * @version 1.0
 *
 * El control se reliza con una salida PWM
 * sobre un circuito de control que convirte dicha
 * salida a una señal 0-10v.
 *
 */

#ifndef __ED_LUMINARY_DRIVER__
#define __ED_LUMINARY_DRIVER__

#include "index.h"

// * Constants
const float dim_sens = 0.95; // Dimming = Curva * dim_sens 0.945682376

class LuminaryDriver
{
private:
  DigitalOut relay;
  PwmOut dimming;

public:
  LuminaryDriver(/* args */);
  ~LuminaryDriver();
  bool getRelay();
  uint8_t getDimming();
  void setRelay(bool value = true);
  void setDimming(uint8_t value = 100);
};

LuminaryDriver::LuminaryDriver(/* args */) : relay(PB_2), dimming(PB_0)
{
  dimming.period_us(1000); //Periodo .1 Segundo
}

LuminaryDriver::~LuminaryDriver()
{
}

#endif
