/**
 * @file    LoraModuleDriver.h
 * @brief   Driver para comuicacion y configuracion LoRa
 * @author  Emir Dagfal
 * @version 1.0
 *
 */

#ifndef __ED_LORA_MODULE_DRIVER__
#define __ED_LORA_MODULE_DRIVER__

#include "dot_util.h"
#include "RadioEvent.h"
#include "index.h"

// * Constants
// mDot *dot = NULL;
// lora::ChannelPlan *plan = NULL;
// extern mDot *dot;
// extern lora::ChannelPlan *plan;

class LoraModuleDriver
{
private:
  std::string network_name;
  std::string network_passphrase;
  uint8_t network_id[8];
  uint8_t network_key[16];
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
  LoraModuleDriver(mDot *d, lora::ChannelPlan *p);
  ~LoraModuleDriver();
  bool getDeepsleep();
  void join();
  bool isJoined();
  void deepSleep();
  void send(std::vector<uint8_t> data);
};

#endif
