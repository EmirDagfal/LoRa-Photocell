#include "dot_util.h"
#include "DotEvent.h"

#if ACTIVE_EXAMPLE == MAIN
static std::string network_name = "MultiTech";
static std::string network_passphrase = "MultiTech";
static uint8_t appEUI[] = {0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x03, 0x7A, 0x1C};
static uint8_t appKey[] = {0x4B, 0xBC, 0xD5, 0xB2, 0x27, 0xD7, 0xF7, 0x5C, 0x13, 0x89, 0xC5, 0x84, 0x8F, 0xF1, 0x21, 0x44};
static uint8_t frequency_sub_band = 2;
static lora::NetworkType network_type = lora::PUBLIC_LORAWAN;
static uint8_t join_delay = 5;
static uint8_t ack = 0;
static bool adr = true;

// deepsleep consumes slightly less current than sleep
// in sleep mode, IO state is maintained, RAM is retained, and application will resume after waking up
// in deepsleep mode, IOs float, RAM is lost, and application will start from beginning after waking up
// if deep_sleep == true, device will enter deepsleep mode
static bool deep_sleep = false;

mDot *dot = NULL;
lora::ChannelPlan *plan = NULL;

DigitalOut led1(LED1);
DigitalOut led2(LED2);

RawSerial pc(USBTX, USBRX, 115200);

void initSetup();
void dlCallback();

int main()
{

  initSetup();

  while (true)
  {

    uint8_t randomValue = rand() % 100;
    static uint8_t progressiveValue = 100;
    progressiveValue += (progressiveValue == 100) ? -100 : 10;
    std::vector<uint8_t> tx_data;

    // join network if not joined
    if (!dot->getNetworkJoinStatus())
    {
      join_network();
    }

    tx_data.push_back(randomValue & 0xFF);
    tx_data.push_back(progressiveValue & 0xFF);
    logInfo("Valor aleatorio: %u [0x%02X]", randomValue, randomValue);
    logInfo("Valor progresivo: %u [0x%02X]", progressiveValue, progressiveValue);
    send_data(tx_data);

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

  return 0;
}

void dlCallback(uint8_t port, uint8_t *payload, uint16_t size, int16_t rssi, int16_t snr)
{
  logInfo("Downlink!!!!!!");
  logInfo("Port: %d, Size: %d", port, size);
  logInfo("RSSI: %d, SNR: %d", rssi, snr);
}

void initSetup()
{
  // Custom event handler for automatically displaying RX data
  static DotEvent events(&dlCallback);

  mts::MTSLog::setLogLevel(mts::MTSLog::TRACE_LEVEL);

#if CHANNEL_PLAN == CP_US915
  lora::ChannelPlan *plan = new lora::ChannelPlan_US915();
#elif CHANNEL_PLAN == CP_AU915
  lora::ChannelPlan *plan = new lora::ChannelPlan_AU915();
#elif CHANNEL_PLAN == CP_EU868
  lora::ChannelPlan *plan = new lora::ChannelPlan_EU868();
#elif CHANNEL_PLAN == CP_KR920
  lora::ChannelPlan *plan = new lora::ChannelPlan_KR920();
#elif CHANNEL_PLAN == CP_AS923
  lora::ChannelPlan *plan = new lora::ChannelPlan_AS923();
#elif CHANNEL_PLAN == CP_AS923_JAPAN
  lora::ChannelPlan *plan = new lora::ChannelPlan_AS923_Japan();
#elif CHANNEL_PLAN == CP_IN865
  lora::ChannelPlan *plan = new lora::ChannelPlan_IN865();
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
    update_ota_config_id_key(appEUI, appKey, frequency_sub_band, network_type, ack);

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
#endif
