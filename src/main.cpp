#include "index.h"
#include "LoraModuleDriver.h"

#if ACTIVE_EXAMPLE == MAIN

mDot *dot = NULL;
lora::ChannelPlan *plan = NULL;

#if defined(TARGET_XDOT_L151CC)
I2C i2c(I2C_SDA, I2C_SCL);
ISL29011 lux(i2c);
#else
AnalogIn lux(XBEE_AD0);
#endif

DigitalOut led1(LED1);
DigitalOut led2(LED2);

RawSerial pc(USBTX, USBRX, 115200);

int main()
{
  mts::MTSLog::setLogLevel(mts::MTSLog::TRACE_LEVEL);
  /**
   *
   *
   */
  LoraModuleDriver loraModule(dot, plan);
  // dot = loraModule.dot;

  while (true)
  {

    uint16_t light;
    std::vector<uint8_t> tx_data;

    loraModule.join();

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
    loraModule.send(tx_data);

    // put the LSL29011 ambient light sensor into a low power state
    lux.setMode(ISL29011::PWR_DOWN);
#else
    // get some dummy data and send it to the gateway
    light = lux.read_u16();
    tx_data.push_back((light >> 8) & 0xFF);
    tx_data.push_back(light & 0xFF);
    logInfo("light: %lu [0x%04X]", light, light);
    loraModule.send(tx_data);
#endif

    loraModule.deepSleep();
  }

  return 0;
}

#endif
