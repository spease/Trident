#ifndef _NETWORKCONNECTION_H_
#define _NETWORKCONNECTION_H_
#include "pins.h"
#include "globals.h"
#include <Adafruit_CC3000.h>

PROGMEM static uint32_t const NETWORKCONNECTION_timeout_ms=10000;
PROGMEM static int const NETWORKCONNECTION_pinIRQ=2;
PROGMEM static int const NETWORKCONNECTION_pinVBAT=A3;
PROGMEM static int const NETWORKCONNECTION_pinCS=8;

class NetworkConnection
{
public:
  NetworkConnection()
  :m_cc(CC3000_SPI_CS, CC3000_IRQ, CC3000_VBAT, SPI_CLOCK_DIV2),m_status(STATUS_DISCONNECTED)
  {}
  
  bool acquire(uint32_t const i_timeStart_ms, uint32_t const i_duration_ms);
  
  Adafruit_CC3000 cc()
  {
    return m_cc;
  }
  
  bool connected() const
  {
    return m_status == STATUS_CONNECTED;
  }
  
  void lost();
  
  void setup()
  {
    TRIDENT_INFO(F("NW"));
    
    pinMode(NETWORKCONNECTION_pinIRQ, INPUT);
    pinMode(NETWORKCONNECTION_pinVBAT, OUTPUT);
    digitalWrite(NETWORKCONNECTION_pinVBAT, HIGH);
    pinMode(NETWORKCONNECTION_pinCS, INPUT);
    m_cc.begin();
  }
  
  void update(uint32_t const i_timeStart_ms, uint32_t const i_budget_ms)
  {
    this->acquire(i_timeStart_ms, i_budget_ms);
  }
private:
  enum Status
  {
    STATUS_DISCONNECTED=0,
    STATUS_SCANNING=1,
    STATUS_ASSOCIATING=2,
    STATUS_REQUESTING=3,
    STATUS_CONNECTED=4
  };
  
  Adafruit_CC3000 m_cc;
  NetworkConnection::Status m_status;
  uint32_t m_timeLastActivity;
};

#endif  /* _NETWORKCONNECTION_H_ */
