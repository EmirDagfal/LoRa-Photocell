/**
 * @file    LuminaryDriver.cpp
 * @brief   Control 0-10v para Luminarias
 * @author  Emir Dagfal
 * @version 1.0
 *
 * El control se reliza con una salida PWM
 * sobre un circuito de control que convirte dicha
 * salida a una señal 0-10v.
 *
 */

#include "LuminaryDriver.h"

bool LuminaryDriver::getRelay()
{
  return (bool)this.relay;
}
uint8_t LuminaryDriver::getDimming()
{
  return (uint8_t)(this.dimming.read() * 100); // value in %
}
LuminaryDriver::setRelay(bool value = true)
{
  this.relay = value;
}
LuminaryDriver::setDimming(uint8_t value = 100) // value in %
{
  if (value <= 0)
  {
    this.relay = 0;
    value = 0;
  }
  this.dimming.pulsewidth_us(value * 10);
}
