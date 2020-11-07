#ifndef __ED_DOT_EVENT_H__
#define __ED_DOT_EVENT_H__

#include "dot_util.h"
#include "mDotEvent.h"
#include "Fota.h"

class DotEvent : public mDotEvent
{

public:
  DotEvent() {}
  DotEvent(std::function<void(uint8_t port, uint8_t *payload, uint16_t size, int16_t rssi, int16_t snr)> callback) : downlinkCallback(callback) {}

  virtual ~DotEvent() {}

  virtual void PacketRx(uint8_t port, uint8_t *payload, uint16_t size, int16_t rssi, int16_t snr, lora::DownlinkControl ctrl, uint8_t slot, uint8_t retries, uint32_t address, bool dupRx);
  virtual void MacEvent(LoRaMacEventFlags *flags, LoRaMacEventInfo *info);
  virtual void ServerTime(uint32_t seconds, uint8_t sub_seconds);

  void downlinkCallback(uint8_t port, uint8_t *payload, uint16_t size, int16_t rssi, int16_t snr);
  void setDownlinkCallback(std::function<void(uint8_t port, uint8_t *payload, uint16_t size, int16_t rssi, int16_t snr)> callback);
};

#endif
