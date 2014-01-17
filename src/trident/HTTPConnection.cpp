#include "HTTPConnection.h"

bool HTTPConnection::acquire(uint32_t const i_timeStart_ms, uint32_t const i_budget_ms)
{
  if(m_domain == NULL)
  {
    return false;
  }
  
  if(!m_network.connected() && m_status != STATUS_OFFLINE)
  {
    m_client.close();
    m_status = STATUS_OFFLINE;
    TRIDENT_INFO(F("HTTP off"));
    return false;
  }
  
  while(m_status != STATUS_CONNECTED)
  {
    if(m_status == STATUS_OFFLINE)
    {
      if(m_network.acquire(i_timeStart_ms, i_budget_ms))
      {
        m_status = STATUS_ONLINE;
        TRIDENT_INFO(F("HTTP On"));
      }
    }
    else if(m_status == STATUS_ONLINE)
    {
      m_network.cc().getHostByName(const_cast<char *>(m_domain), &m_ip);
      m_status = STATUS_RESOLVING;
      m_timeResolvingStart = millis();
      TRIDENT_INFO(F("HTTP DNS"));
    }
    else if(m_status > STATUS_ONLINE && timeSince_ms(m_timeResolvingStart) > msc_timeoutNetwork_ms)
    {
      /***** Maybe network is down *****/
      this->lostTotally();
    }
    else if(m_status == STATUS_RESOLVING)
    {
      if(m_network.cc().getHostByName(const_cast<char *>(m_domain), &m_ip))
      {
        if(m_ip != 0)
        {
          m_status = STATUS_RESOLVED;
          TRIDENT_INFO(F("HTTP Res"));
        }
        else
        {
          m_status = STATUS_ONLINE;
          TRIDENT_INFO(F("HTTP On"));
        }
      }
    }
    else if(m_status == STATUS_RESOLVED)
    {
      m_client = m_network.cc().connectTCP(m_ip, msc_httpPort);
      m_status = STATUS_OPENING;
      TRIDENT_INFO(F("HTTP Opn"));
    }
    else if(m_status == STATUS_OPENING)
    {
      if(m_client.connected())
      {
        m_status = STATUS_CONNECTED;
        TRIDENT_INFO(F("HTTP Cnt"));
      }
    }
    
    /***** Check time *****/
    if(timeSince_ms(i_timeStart_ms) > i_budget_ms)
    {
      return false;
    }
  }
  
  return true;
}
bool HTTPConnection::writePost(__FlashStringHelper const * const i_uri, char const * const i_data, uint32_t const i_timeStart_ms, uint32_t const i_budget_ms)
{
  if(!this->acquire(i_timeStart_ms, i_budget_ms))
  {
    return false;
  }

  m_client.fastrprint(F("POST "));
  m_client.fastrprint(i_uri);
  m_client.fastrprintln(F(" HTTP/1.1"));
  
  if(m_authBase64 != NULL)
  {
    m_client.fastrprint(F("Authorization: Basic "));
    m_client.fastrprintln(m_authBase64);
  }
  
  m_client.fastrprint(F("Host: "));
  m_client.fastrprintln(m_domain);
  
  m_client.fastrprint(F("Content-Length: "));
  m_client.println(strlen(i_data));
  
  m_client.fastrprintln(F("Content-Type:application/json"));
  m_client.println();
  
  m_client.fastrprintln(i_data);
  
  m_timeLastWrite=millis();
  this->setResponseExpected(true);
  return true;
}
