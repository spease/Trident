#ifndef _HTTPCONNECTION_H_
#define _HTTPCONNECTION_H_

#include "Socket.h"
#include "NetworkConnection.h"

#include "globals.h"

typedef char const *(*HTTPCallback)(void * io_callbackPointer);

class HTTPConnection
{
public:
  HTTPConnection(__FlashStringHelper const * const i_domain, __FlashStringHelper const * const i_authBase64)
  :m_authBase64(i_authBase64),m_domain(i_domain),m_flags(FLAG_DEFAULT),m_timeOpeningStart(0),m_timeResolvingStart(0),m_timeLastRead(0),m_timeLastWrite(0)
  {
  }
  
  bool acquire(uint32_t const i_timeStart_ms, uint32_t const i_budget_ms);
  
  void setup()
  {
    m_network.setup();
    
    TRIDENT_INFO("HTTP");
  }
  
  void update(uint32_t const i_timeStart_ms, uint32_t const i_budget_ms)
  {
    size_t dataRead=0;
    m_network.update(i_timeStart_ms, i_budget_ms);
    
    cc3k_int_poll();
    
    while(timeSince_ms(i_timeStart_ms) < i_budget_ms)
    {
      if(!this->acquire(i_timeStart_ms, i_budget_ms))
      {
        continue;
      }
      else if(m_socket.available())
      {
        m_socket.read();
        ++dataRead;
      }
    }
    
    if(dataRead)
    {
      TRIDENT_INFO(F("HTTP Read: "));
      TRIDENT_INFO(dataRead);
      m_timeLastRead = millis();
      this->setResponseExpected(false);
    }
  }
  
  bool writePost(__FlashStringHelper const * const i_uri, char const * const i_data, uint32_t const i_timeStart_ms, uint32_t const i_budget_ms);
protected:
  void lost();
  void lostTotally();
  bool responseExpected() const
  {
    return (m_flags & FLAG_RESPONSE_EXPECTED);
  }
  void setResponseExpected(bool const i_responseExpected)
  {
    if(i_responseExpected)
    {
      m_flags |= FLAG_RESPONSE_EXPECTED;
    }
    else
    {
      m_flags &= (~FLAG_RESPONSE_EXPECTED);
    }
  }
private:
  static uint32_t const msc_timeoutNetwork_ms=10000;
  static uint32_t const msc_timeoutConnection_ms=5000;
  static int const msc_httpPort=80;
  
  enum Flags
  {
    FLAG_DEFAULT=0,
    FLAG_RESPONSE_EXPECTED=(1<<0)
  };
  
  enum Status
  {
    STATUS_OFFLINE=0,
    STATUS_ONLINE,
    STATUS_RESOLVING,
    STATUS_RESOLVED,
    STATUS_OPENING,
    STATUS_CONNECTED
  };
  
  __FlashStringHelper const * const m_authBase64;
  //Adafruit_CC3000_Client m_socket;
  Socket m_socket;
  __FlashStringHelper const * const m_domain;
  uint8_t m_flags;
  uint32_t m_ip;
  NetworkConnection m_network;
  Status m_status;
  uint32_t m_timeOpeningStart;
  uint32_t m_timeResolvingStart;
  uint32_t m_timeLastRead;
  uint32_t m_timeLastWrite;
};

#endif
