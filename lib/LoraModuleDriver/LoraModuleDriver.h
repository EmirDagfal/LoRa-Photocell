/**
 * @file    LoraModuleDriver.h
 * @brief   Driver para comuicacion y configuracion LoRa
 * @author  Emir Dagfal
 * @version 1.0
 *
 *
 */

#ifndef __ED_LORA_MODULE_DRIVER__
#define __ED_LORA_MODULE_DRIVER__

#include "index.h"

// * Constants

class LoraModuleDriver
{
private:
  std::string network_name;
  std::string network_passphrase;
  uint8_t network_id[];
  uint8_t network_key[];
  uint8_t frequency_sub_band;
  lora::NetworkType network_type;
  uint8_t join_delay;
  uint8_t ack;
  bool adr;

  // deepsleep consumes slightly less current than sleep
  // in sleep mode, IO state is maintained, RAM is retained, and application will resume after waking up
  // in deepsleep mode, IOs float, RAM is lost, and application will start from beginning after waking up
  // if deep_sleep == true, device will enter deepsleep mode
  bool deep_sleep;

  mDot *dot;
  lora::ChannelPlan *plan;

  void initConfig();

public:
  LoraModuleDriver(/* args */);
  ~LoraModuleDriver();
};

LoraModuleDriver::LoraModuleDriver(/* args */)
{
  // * Inicializacion de propiedades
  network_name = "MultiTech";
  network_passphrase = "MultiTech";
  network_id[] = {0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x03, 0x5B, 0x5C};
  network_key[] = {0x25, 0xCB, 0xCF, 0x9B, 0x80, 0x8D, 0x8C, 0xC3, 0xE1, 0xB2, 0x67, 0xFA, 0xC4, 0xE0, 0x6D, 0x76};
  frequency_sub_band = 2;
  network_type = lora::PUBLIC_LORAWAN;
  join_delay = 5;
  ack = 0;
  adr = true;

  // deepsleep consumes slightly less current than sleep
  // in sleep mode, IO state is maintained, RAM is retained, and application will resume after waking up
  // in deepsleep mode, IOs float, RAM is lost, and application will start from beginning after waking up
  // if deep_sleep == true, device will enter deepsleep mode
  deep_sleep = false;

  *dot = NULL;
  *plan = NULL;

  // * Rutina de configuracion
  initConfig();
}

LoraModuleDriver::~LoraModuleDriver()
{
}

#endif
