#ifndef __ED_PHOTOCELL__
#define __ED_PHOTOCELL__

#include "mbed.h"

enum MODE : uint8_t
{
  PHOTOCELL,
  HOUR,
  MANUAL,
  DEMO
};

class Photocell
{
private:
  uint8_t mode;

public:
  Photocell(/* args */);
  ~Photocell();
};

Photocell::Photocell(/* args */)
{
  mode = MODE::PHOTOCELL;
}

Photocell::~Photocell()
{
}

#endif
