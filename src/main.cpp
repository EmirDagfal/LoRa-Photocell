#include "DotEvent.h"

#if ACTIVE_EXAMPLE == MAIN

#if defined(TARGET_XDOT_L151CC)
I2C i2c(I2C_SDA, I2C_SCL);
ISL29011 lux(i2c);
#else
AnalogIn lux(XBEE_AD0);
#endif

DigitalOut led1(LED1);
DigitalOut led2(LED2);

RawSerial pc(USBTX, USBRX, 115200);
RawSerial serialIn(UART_TX, UART_RX, 115200);

// deepsleep consumes slightly less current than sleep
// in sleep mode, IO state is maintained, RAM is retained, and application will resume after waking up
// in deepsleep mode, IOs float, RAM is lost, and application will start from beginning after waking up
// if deep_sleep == true, device will enter deepsleep mode
static bool deep_sleep = false;

int main()
{
#if defined(TARGET_XDOT_L151CC)
  i2c.frequency(400000);
#endif
  loraConfiguration();

  while (true)
  {

    uint16_t light;
    std::vector<uint8_t> tx_data;

    // join network if not joined
    if (!dot->getNetworkJoinStatus())
    {
      join_network();
    }

#if defined(TARGET_XDOT_L151CC)
    // configure the ISL29011 sensor on the xDot-DK for continuous ambient light sampling, 16 bit conversion, and maximum range
    lux.setMode(ISL29011::ALS_CONT);
    lux.setResolution(ISL29011::ADC_16BIT);
    lux.setRange(ISL29011::RNG_64000);

    // get the latest light sample and send it to the gateway
    light = lux.getData();
    tx_data.push_back((light >> 8) & 0xFF);
    tx_data.push_back(light & 0xFF);
    logInfo("light: %lu [0x%04X]", light, light);
    send_data(tx_data);

    // put the LSL29011 ambient light sensor into a low power state
    lux.setMode(ISL29011::PWR_DOWN);
#else
    // get some dummy data and send it to the gateway
    light = lux.read_u16();
    tx_data.push_back((light >> 8) & 0xFF);
    tx_data.push_back(light & 0xFF);
    logInfo("light: %lu [0x%04X]", light, light);
    send_data(tx_data);
#endif

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

#endif
