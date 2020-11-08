#ifndef __ED_DOT_EVENT_H__
#define __ED_DOT_EVENT_H__

#include "dot_util.h"
#include "RadioEvent.h"
#include "Fota.h"

class DotEvent : public RadioEvent
{
  bool isDownlinkCallbackConfigured;

protected:
  void (*downlinkCallback)(uint8_t port, uint8_t *payload, uint16_t size, int16_t rssi, int16_t snr);

public:
  DotEvent();
  DotEvent(void (*downlinkCallback_)(uint8_t, uint8_t *, uint16_t, int16_t, int16_t));

  virtual ~DotEvent();

  void setDownlinkCallback(void (*downlinkCallback_)(uint8_t, uint8_t *, uint16_t, int16_t, int16_t));
  virtual void PacketRx(uint8_t port, uint8_t *payload, uint16_t size, int16_t rssi, int16_t snr, lora::DownlinkControl ctrl, uint8_t slot, uint8_t retries, uint32_t address, bool dupRx)
  {

    RadioEvent::PacketRx(port, payload, size, rssi, snr, ctrl, slot, retries, address, dupRx);
    downlinkCallback(port, payload, size, rssi, snr);
  }
};

#endif
