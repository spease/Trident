#include "HTTPConnection.h"

bool HTTPConnection::acquire(uint32_t const i_timeStart_ms, uint32_t const i_budget_ms)
{
  if(m_domain == NULL)
  {
    return false;
  }
  
  while(timeSince_ms(i_timeStart_ms) < i_budget_ms)
  {
    if(m_status == STATUS_OFFLINE)
    {
      if(m_network.acquire(i_timeStart_ms, i_budget_ms))
      {
        m_status = STATUS_ONLINE;
        TRIDENT_INFO(F("HTTP On"));
      }
    }
    else if(!m_network.connected())
    {
      m_socket.close();
      m_status = STATUS_OFFLINE;
      TRIDENT_INFO(F("HTTP off"));
    }
    else if(m_status == STATUS_ONLINE)
    {
      m_status = STATUS_RESOLVING;
      m_timeResolvingStart = millis();
      TRIDENT_INFO(F("HTTP DNS"));
    }
    else if(m_status < STATUS_CONNECTED && timeSince_ms(m_timeResolvingStart) > msc_timeoutNetwork_ms)
    {
      /***** Maybe network is down *****/
      this->lostTotally();
    }
    else if(m_status == STATUS_RESOLVING)
    {
      char domainName[32];
      size_t i=0;
      do
      {
        domainName[i] = pgm_read_byte(&reinterpret_cast<char const PROGMEM *>(m_domain)[i]);
      } while(domainName[i++] != '\0');
      TRIDENT_INFO(domainName);
      if(m_network.cc().getHostByName(domainName, &m_ip))
      {
        if(m_ip != 0)
        {
          m_status = STATUS_RESOLVED;
          TRIDENT_INFO(F("HTTP Res"));
        }
      }
    }
    else if(m_status == STATUS_RESOLVED)
    {
      m_socket.connect(m_ip, msc_httpPort);
      //m_socket = m_network.cc().connectTCP(m_ip, msc_httpPort);
      m_timeOpeningStart = millis();
      m_status = STATUS_OPENING;
      TRIDENT_INFO(F("HTTP Opn"));
    }
    else if(m_status == STATUS_OPENING)
    {
      if(m_socket.connected())
      {
        m_timeLastRead = millis();
        m_timeLastWrite = millis();
        m_status = STATUS_CONNECTED;
        TRIDENT_INFO(F("HTTP Cnt"));
      }
      else if(timeSince_ms(m_timeOpeningStart) > msc_timeoutConnection_ms)
      {
        m_socket.close();
        m_status = STATUS_RESOLVED;
      }
    }
    else if(m_status == STATUS_CONNECTED)
    {
      if(!m_socket.connected())
      {
        this->lost();
      }
      /*
      else if(this->responseExpected() && timeSince_ms(m_timeLastRead) > msc_timeoutConnection_ms)
      {
        this->lost();
      }
      */
      else
      {
        return true;
      }
    }
  }
  
  return false;
}

void HTTPConnection::lost()
{
  m_socket.close();
  m_status = STATUS_ONLINE;
  TRIDENT_INFO(F("HTTP lost"));
}

void HTTPConnection::lostTotally()
{
  m_socket.close();
  m_status = STATUS_OFFLINE;
  TRIDENT_INFO("HTTP totally lost");
  m_network.lost();
}

bool HTTPConnection::writePost(__FlashStringHelper const * const i_uri, char const * const i_data, uint32_t const i_timeStart_ms, uint32_t const i_budget_ms)
{
  if(!this->acquire(i_timeStart_ms, i_budget_ms))
  {
    return false;
  }
  this->update(i_timeStart_ms, i_budget_ms);

  m_socket.print(F("POST "));
  m_socket.print(i_uri);
  m_socket.println(F(" HTTP/1.1"));
  
  if(m_authBase64 != NULL)
  {
    m_socket.print(F("Authorization: Basic "));
    m_socket.println(m_authBase64);
  }
  
  m_socket.println(F("User-Agent: curl/7.34.0"));
  
  m_socket.print(F("Host: "));
  m_socket.println(F("api.tempo-db.com"));
  
  m_socket.println(F("Accept: */*"));

  m_socket.println(F("Content-Type:application/json"));
  
  m_socket.print(F("Content-Length: "));
  m_socket.println(strlen(i_data));
  m_socket.println();
  
  m_socket.print(i_data);
  
  m_timeLastWrite=millis();
  this->setResponseExpected(true);
  return true;
}
