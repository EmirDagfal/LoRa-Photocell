#ifndef __RADIO_EVENT_H__
#define __RADIO_EVENT_H__

#include "dot_util.h"
#include "mDotEvent.h"
#include "Fota.h"

class RadioEvent : public mDotEvent
{
  /**
   * @brief Miembros de clase añadidos o modificados
   * @author Emir Dagfal
   */
  bool isDownlinkCallbackConfigured;

protected:
  void (*downlinkCallback)(uint8_t port, uint8_t *payload, uint16_t size, int16_t rssi, int16_t snr);

public:
  RadioEvent() : isDownlinkCallbackConfigured(false) {}
  RadioEvent(void (*dlCB)(uint8_t, uint8_t *, uint16_t, int16_t, int16_t))
      : isDownlinkCallbackConfigured(true),
        downlinkCallback(dlCB) {}
  void setDownlinkCallback(void (*downlinkCallback_)(uint8_t, uint8_t *, uint16_t, int16_t, int16_t))
  {
    downlinkCallback = downlinkCallback_;
    isDownlinkCallbackConfigured = true;
  }

  virtual ~RadioEvent() {}

  virtual void PacketRx(uint8_t port, uint8_t *payload, uint16_t size, int16_t rssi, int16_t snr, lora::DownlinkControl ctrl, uint8_t slot, uint8_t retries, uint32_t address, bool dupRx)
  {
    mDotEvent::PacketRx(port, payload, size, rssi, snr, ctrl, slot, retries, address, dupRx);

#if ACTIVE_EXAMPLE == FOTA_EXAMPLE
    if (port == 200 || port == 201 || port == 202)
    {
      Fota::getInstance()->processCmd(payload, port, size);
    }
#else
    downlinkCallback(port, payload, size, rssi, snr);
#endif
  }
  /**
 *  --o---
 */

  /*!
     * MAC layer event callback prototype.
     *
     * \param [IN] flags Bit field indicating the MAC events occurred
     * \param [IN] info  Details about MAC events occurred
     */
  virtual void MacEvent(LoRaMacEventFlags *flags, LoRaMacEventInfo *info)
  {

    if (mts::MTSLog::getLogLevel() == mts::MTSLog::TRACE_LEVEL)
    {
      std::string msg = "OK";
      switch (info->Status)
      {
      case LORAMAC_EVENT_INFO_STATUS_ERROR:
        msg = "ERROR";
        break;
      case LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT:
        msg = "TX_TIMEOUT";
        break;
      case LORAMAC_EVENT_INFO_STATUS_RX_TIMEOUT:
        msg = "RX_TIMEOUT";
        break;
      case LORAMAC_EVENT_INFO_STATUS_RX_ERROR:
        msg = "RX_ERROR";
        break;
      case LORAMAC_EVENT_INFO_STATUS_JOIN_FAIL:
        msg = "JOIN_FAIL";
        break;
      case LORAMAC_EVENT_INFO_STATUS_DOWNLINK_FAIL:
        msg = "DOWNLINK_FAIL";
        break;
      case LORAMAC_EVENT_INFO_STATUS_ADDRESS_FAIL:
        msg = "ADDRESS_FAIL";
        break;
      case LORAMAC_EVENT_INFO_STATUS_MIC_FAIL:
        msg = "MIC_FAIL";
        break;
      default:
        break;
      }
      logTrace("Event: %s", msg.c_str());

      logTrace("Flags Tx: %d Rx: %d RxData: %d RxSlot: %d LinkCheck: %d JoinAccept: %d",
               flags->Bits.Tx, flags->Bits.Rx, flags->Bits.RxData, flags->Bits.RxSlot, flags->Bits.LinkCheck, flags->Bits.JoinAccept);
      logTrace("Info: Status: %d ACK: %d Retries: %d TxDR: %d RxPort: %d RxSize: %d RSSI: %d SNR: %d Energy: %d Margin: %d Gateways: %d",
               info->Status, info->TxAckReceived, info->TxNbRetries, info->TxDatarate, info->RxPort, info->RxBufferSize,
               info->RxRssi, info->RxSnr, info->Energy, info->DemodMargin, info->NbGateways);
    }

    if (flags->Bits.Rx)
    {

      logDebug("Rx %d bytes", info->RxBufferSize);
      if (info->RxBufferSize > 0)
      {
#if ACTIVE_EXAMPLE != FOTA_EXAMPLE
        // print RX data as string and hexadecimal
        std::string rx((const char *)info->RxBuffer, info->RxBufferSize);
        printf("Rx data: %s [%s]\r\n", rx.c_str(), mts::Text::bin2hexString(info->RxBuffer, info->RxBufferSize).c_str());
#endif
      }
    }

    // * Send to callback
    if (info->RxBufferSize > 0 && isDownlinkCallbackConfigured)
    {
      /**        LoRaMacEventInfoStatus Status;
        lora::DownlinkControl Ctrl;
        bool TxAckReceived;
        bool DuplicateRx;
        uint8_t TxNbRetries;
        uint8_t TxDatarate;
        uint8_t RxPort;
        uint8_t *RxBuffer;
        uint8_t RxBufferSize;
        int16_t RxRssi;
        uint8_t RxSnr;
        uint16_t Energy;
        uint8_t DemodMargin;
        uint8_t NbGateways;
        */
      downlinkCallback(info->RxPort, info->RxBuffer, info->RxBufferSize, info->RxRssi, info->RxSnr);
    }
  }

#if ACTIVE_EXAMPLE == FOTA_EXAMPLE
  virtual void ServerTime(uint32_t seconds, uint8_t sub_seconds)
  {
    mDotEvent::ServerTime(seconds, sub_seconds);

    Fota::getInstance()->setClockOffset(seconds);
  }
#endif
};

#endif
