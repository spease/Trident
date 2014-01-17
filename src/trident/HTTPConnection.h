#ifndef _HTTPCONNECTION_H_
#define _HTTPCONNECTION_H_

#include "NetworkConnection.h"

#include "globals.h"

//#include <Base64.h>

typedef char const *(*HTTPCallback)(void * io_callbackPointer);

class HTTPConnection
{
public:
  HTTPConnection(char const * const i_domain, __FlashStringHelper const * const i_authBase64)
  :m_authBase64(i_authBase64),m_domain(i_domain),m_flags(FLAG_DEFAULT)
  {
  }
  
  ~HTTPConnection()
  {
  }
  
  bool acquire(uint32_t const i_timeStart_ms, uint32_t const i_budget_ms);
  
  void lost()
  {
    m_client.close();
    m_status = STATUS_ONLINE;
    TRIDENT_INFO(F("HTTP lost"));
  }
  
  void lostTotally()
  {
    m_client.close();
    m_status = STATUS_OFFLINE;
    TRIDENT_INFO("HTTP off");
    m_network.lost();
  }
#if 0
  bool read(uint32_t const i_timeStart_ms, uint32_t const i_budget_ms)
  {
    while(true)
    {
      /***** Check time *****/
      if(timeSince_ms(i_timeStart_ms) > i_budget_ms)
      {
        return false;
      }
      
      if(!this->acquire(i_timeStart_ms, i_budget_ms))
      {
        continue;
      }
      else if(!m_client.connected())
      {
        this->lost();
        continue;
      }
      else if(!m_client.available())
      {
        if(this->responseExpected() && timeSince_ms(m_timeLastWrite) > msc_timeoutConnection_ms)
        {
          this->lost();
        }
        continue;
      }
      
      while(m_client.available())
      {
        m_client.read();
        this->setResponseExpected(false);
        
        /***** Check time *****/
        if(timeSince_ms(i_timeStart_ms) > i_budget_ms)
        {
          return true;
        }
      }
      
      return true;
    }
  }
#endif
  bool responseExpected() const
  {
    return (m_flags & FLAG_RESPONSE_EXPECTED);
  }
  
  /*
  bool setAuthenticationBasic(char const * const i_authString)
  {
    if(i_authString != NULL)
    {
      size_t const authStringLength = strlen(i_authString);
      
      //http://stackoverflow.com/questions/1533113/calculate-the-size-to-a-base-64-encoded-message
      size_t const b64CodeLength = ((authStringLength*4)/3);
      size_t const b64PaddingLength = ((authStringLength%3) ? (3-(authStringLength%3)) : 0);
      size_t const b64CRLFLength = 2+(2*(authStringLength+b64PaddingLength)/72);
      size_t const b64TotalSizeBytes = b64CodeLength + b64PaddingLength + b64CRLFLength + 1;
      char * const b64 = new char[b64TotalSizeBytes];
      if(b64 == NULL)
      {
        return false;
      }
      if(m_authBase64 != NULL)
      {
        delete[] m_authBase64;
      }
      m_authBase64 = b64;
      
      base64_encode(m_authBase64, const_cast<char *>(i_authString), authStringLength);
      m_flags |= FLAG_AUTH_BASIC;
      TRIDENT_INFO("HTTP Basic");
      return true;
    }
    else
    {
      m_flags &= ~FLAG_AUTH_BASIC;
      if(m_authBase64 != NULL)
      {
        delete[] m_authBase64;
      }
      TRIDENT_INFO("HTTP None");
    }
  }
  */
  
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
  
  void setup()
  {
    m_network.setup();
    
    TRIDENT_INFO("HTTP");
  }
  
  void update(uint32_t const i_timeStart_ms, uint32_t const i_budget_ms)
  {
    m_network.update(i_timeStart_ms, i_budget_ms);
    this->acquire(i_timeStart_ms, i_budget_ms);
  }
  
  bool writePost(const __FlashStringHelper * const i_uri, char const * const i_data, uint32_t const i_timeStart_ms, uint32_t const i_budget_ms);
private:
  static uint32_t const msc_timeoutNetwork_ms=5000;
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
  Adafruit_CC3000_Client m_client;
  char const * const m_domain;
  uint8_t m_flags;
  uint32_t m_ip;
  NetworkConnection m_network;
  Status m_status;
  uint32_t m_timeResolvingStart;
  uint32_t m_timeLastWrite;
};

#endif
