#include "DotEvent.h"

#if ACTIVE_EXAMPLE == MAIN

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
  loraConfiguration();

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

#endif
