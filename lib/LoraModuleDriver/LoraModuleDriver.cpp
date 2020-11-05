/**
 * @file    LoraModuleDriver.cpp
 * @brief   Driver para comuicacion y configuracion LoRa
 * @author  Emir Dagfal
 * @version 1.0
 *
 */
#include "LoraModuleDriver.h"

LoraModuleDriver::LoraModuleDriver(mDot *d, lora::ChannelPlan *p)
    : network_id{0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x03, 0x5B, 0x5C},
      network_key{0x25, 0xCB, 0xCF, 0x9B, 0x80, 0x8D, 0x8C, 0xC3, 0xE1, 0xB2, 0x67, 0xFA, 0xC4, 0xE0, 0x6D, 0x76},
      dot(d),
      plan(p)

{
  // * Inicializacion de propiedades
  network_name = "MultiTech";
  network_passphrase = "MultiTech";
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

  // * Rutina de configuracion
  initConfig();
}

LoraModuleDriver::~LoraModuleDriver()
{
}

void LoraModuleDriver::initConfig()
{
  // Custom event handler for automatically displaying RX data
  RadioEvent events;

  // #if defined(TARGET_XDOT_L151CC)
  //   i2c.frequency(400000);
  // #endif

#if CHANNEL_PLAN == CP_US915
  plan = new lora::ChannelPlan_US915();
#elif CHANNEL_PLAN == CP_AU915
  plan = new lora::ChannelPlan_AU915();
#elif CHANNEL_PLAN == CP_EU868
  plan = new lora::ChannelPlan_EU868();
#elif CHANNEL_PLAN == CP_KR920
  plan = new lora::ChannelPlan_KR920();
#elif CHANNEL_PLAN == CP_AS923
  plan = new lora::ChannelPlan_AS923();
#elif CHANNEL_PLAN == CP_AS923_JAPAN
  plan = new lora::ChannelPlan_AS923_Japan();
#elif CHANNEL_PLAN == CP_IN865
  plan = new lora::ChannelPlan_IN865();
#endif
  assert(plan);

  dot = mDot::getInstance(plan);
  assert(dot);

  // attach the custom events handler
  dot->setEvents(&events);

  if (!dot->getStandbyFlag() && !dot->getPreserveSession())
  {
    logInfo("mbed-os library version: %d.%d.%d", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    // start from a well-known state
    logInfo("defaulting Dot configuration");
    dot->resetConfig();
    dot->resetNetworkSession();

    // make sure library logging is turned on
    dot->setLogLevel(mts::MTSLog::INFO_LEVEL);

    // update configuration if necessary
    if (dot->getJoinMode() != mDot::OTA)
    {
      logInfo("changing network join mode to OTA");
      if (dot->setJoinMode(mDot::OTA) != mDot::MDOT_OK)
      {
        logError("failed to set network join mode to OTA");
      }
    }

    // To preserve session over power-off or reset enable this flag
    // dot->setPreserveSession(true);

    // in OTA and AUTO_OTA join modes, the credentials can be passed to the library as a name and passphrase or an ID and KEY
    // only one method or the other should be used!
    // network ID = crc64(network name)
    // network KEY = cmac(network passphrase)
    //         update_ota_config_name_phrase(network_name, network_passphrase, frequency_sub_band, network_type, ack);
    update_ota_config_id_key(network_id, network_key, frequency_sub_band, network_type, ack);

    // configure the Dot for class C operation
    // the Dot must also be configured on the gateway for class C
    // use the lora-query application to do this on a Conduit: http://www.multitech.net/developer/software/lora/lora-network-server/
    // to provision your Dot for class C operation with a 3rd party gateway, see the gateway or network provider documentation
    logInfo("changing network mode to class C");
    if (dot->setClass("C") != mDot::MDOT_OK)
    {
      logError("failed to set network mode to class C");
    }

    // configure network link checks
    // network link checks are a good alternative to requiring the gateway to ACK every packet and should allow a single gateway to handle more Dots
    // check the link every count packets
    // declare the Dot disconnected after threshold failed link checks
    // for count = 3 and threshold = 5, the Dot will ask for a link check response every 5 packets and will consider the connection lost if it fails to receive 3 responses in a row
    //        update_network_link_check_config(3, 5);

    // enable or disable Adaptive Data Rate
    dot->setAdr(adr);

    // Configure the join delay
    dot->setJoinDelay(join_delay);

    // save changes to configuration
    logInfo("saving configuration");
    if (!dot->saveConfig())
    {
      logError("failed to save configuration");
    }

    // display configuration
    display_config();
  }
  else
  {
    // restore the saved session if the dot woke from deepsleep mode
    // useful to use with deepsleep because session info is otherwise lost when the dot enters deepsleep
    logInfo("restoring network session from NVM");
    dot->restoreNetworkSession();
  }
}

bool LoraModuleDriver::getDeepsleep()
{
  return deep_sleep;
}
void LoraModuleDriver::join()
{
  // join network if not joined
  if (!isJoined())
  {
    join_network();
  }
}

bool LoraModuleDriver::isJoined()
{
  return dot->getNetworkJoinStatus();
}

void LoraModuleDriver::send(std::vector<uint8_t> data)
{
  send_data(data);
}
void LoraModuleDriver::deepSleep()
{
  // if going into deepsleep mode, save the session so we don't need to join again after waking up
  // not necessary if going into sleep mode since RAM is retained
  if (deep_sleep)
  {
    logInfo("saving network session to NVM");
    dot->saveNetworkSession();
  }

  // ONLY ONE of the three functions below should be uncommented depending on the desired wakeup method
  //sleep_wake_rtc_only(deep_sleep);
  //sleep_wake_interrupt_only(deep_sleep);
  sleep_wake_rtc_or_interrupt(deep_sleep);
}
