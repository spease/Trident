#ifndef _HTTPCONNECTION_H_
#define _HTTPCONNECTION_H_

#include "NetworkConnection.h"

#include <Base64.h>

typedef char const *(*HTTPCallback)(void * io_callbackPointer);

class HTTPConnection
{
public:
  HTTPConnection(char const * const i_domain, NetworkConnection &i_network)
  :m_domain(NULL),m_flags(FLAG_DEFAULT),m_network(&i_network),m_responseExpected(false)
  {
    size_t const domainSizeBytes = strlen(i_domain)+1;
    m_domain = new char[domainSizeBytes];
    if(m_domain != NULL)
    {
      memcpy(m_domain, i_domain, domainSizeBytes);
    }
  }
  
  ~HTTPConnection()
  {
    if(m_domain != NULL)
    {
      delete[] m_domain;
      m_domain = NULL;
    }
  }
  
  bool acquire(uint32_t const i_budget_ms)
  {
    uint32_t timeStart = millis();
    
    if(m_domain == NULL)
    {
      return false;
    }
    
    while(m_status != STATUS_CONNECTED)
    {
      if(m_status == STATUS_DISCONNECTED)
      {
        m_network->cc().getHostByName(m_domain, &m_ip);
        m_status = STATUS_RESOLVING;
        m_timeResolvingStart = millis();
        Serial.println("HTP: Resolving '"+String(m_domain)+"'...");
      }
      else if(m_status == STATUS_RESOLVING)
      {
        if(m_network->cc().getHostByName(m_domain, &m_ip))
        {
          m_status = STATUS_RESOLVED;
          m_timeResolvedStart = millis();
          Serial.println("HTP: Resolved '"+String(m_domain)+"' to "+String(m_ip));
        }
      }
      else if(m_status == STATUS_RESOLVED)
      {
        m_client = m_network->cc().connectTCP(m_ip, msc_httpPort);
        m_status = STATUS_OPENING;
        m_timeOpeningStart = millis();
        Serial.println("HTP: Opening "+String(m_ip)+":"+String(msc_httpPort)+"...");
      }
      else if(m_status == STATUS_OPENING)
      {
        if(m_client.connected())
        {
          m_status = STATUS_CONNECTED;
          m_timeConnectedStart = millis();
          Serial.println("Connected to '"+String(m_domain)+"'");
        }
      }
      
      /***** Check time *****/
      if(timeSince_ms(timeStart) > i_budget_ms)
      {
        return false;
      }
      
      /***** Maybe network is down *****/
      if(timeSince_ms(m_timeResolvingStart) > msc_timeoutNetwork_ms)
      {
        this->lost();
        m_network->lost();
      }
    }
    
    return true;
  }
  
  void lost()
  {
    m_client.close();
    m_status = STATUS_DISCONNECTED;
    m_timeDisconnectedStart = millis();
    Serial.println("HTP: Connection lost.");
  }
  
  bool read(uint32_t const i_budget_ms)
  {
    uint32_t timeStart = millis();
    
    while(true)
    {
      /***** Check time *****/
      if(timeSince_ms(timeStart) > i_budget_ms)
      {
        return false;
      }
      
      if(!this->acquire(i_budget_ms))
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
        if(m_responseExpected && timeSince_ms(m_timeLastWrite) > msc_timeoutConnection_ms)
        {
          this->lost();
        }
        continue;
      }
      
      while(m_client.available())
      {
        m_client.read();
        m_responseExpected = false;
        
        /***** Check time *****/
        if(timeSince_ms(timeStart) > i_budget_ms)
        {
          return true;
        }
      }
      
      return true;
    }
  }
  
  void setAuthenticationBasic(char const * const i_authString)
  {
    if(i_authString != NULL)
    {
      size_t const authStringLength = strlen(i_authString);
      size_t const authStringSizeBytes = authStringLength+1;
      char *authString = new char[authStringLength];
      if(authString == NULL)
      {
        return;
      }
      
      memcpy(authString, i_authString, authStringSizeBytes);
      base64_encode(m_authBase64, authString, authStringLength);
      delete[] authString;
      m_flags |= FLAG_AUTH_BASIC;
      Serial.println("HTP: Using basic authentication.");
    }
    else
    {
      m_flags &= ~FLAG_AUTH_BASIC;
      Serial.println("HTP: Using no authentication.");
    }
  }
  
  void setup()
  {
    this->acquire(msc_timeoutConnection_ms);
  }
  
  void update(uint32_t i_budget_ms)
  {
    this->acquire(i_budget_ms);
  }
  
  void update()
  {
  }
  
  bool writePost(char const * const i_uri, char const * const i_data, uint32_t const i_budget_ms)
  {
    if(!this->acquire(i_budget_ms))
    {
      return false;
    }
    
    m_client.fastrprint("POST ");
    m_client.fastrprint(i_uri);
    m_client.fastrprintln(" HTTP/1.1");
    
    if(m_flags & FLAG_AUTH_BASIC)
    {
      m_client.fastrprint("Authorization: Basic ");
      m_client.fastrprintln(m_authBase64);
    }
    
    m_client.fastrprint("Host: ");
    m_client.fastrprintln(m_domain);
    
    m_client.fastrprint("Content-Length: ");
    m_client.println(strlen(i_data));
    
    m_client.fastrprintln("Content-Type:application/json");
    m_client.println();
    
    m_client.fastrprintln(i_data);
    
    m_timeLastWrite=millis();
    m_responseExpected = true;
    return true;
  }
  
private:
  static uint32_t const msc_timeoutNetwork_ms=5000;
  static uint32_t const msc_timeoutConnection_ms=5000;
  static int const msc_httpPort=80;
  
  enum Flags
  {
    FLAG_DEFAULT=0,
    FLAG_AUTH_BASIC=(1<<0),
  };
  
  enum Status
  {
    STATUS_DISCONNECTED=0,
    STATUS_RESOLVING=1,
    STATUS_RESOLVED=2,
    STATUS_OPENING=3,
    STATUS_CONNECTED=4
  };
  
  char m_authBase64[256];
  Adafruit_CC3000_Client m_client;
  char *m_domain;
  uint32_t m_flags;
  uint32_t m_ip;
  NetworkConnection *m_network;
  bool m_responseExpected;
  HTTPConnection::Status m_status;
  uint32_t m_timeDisconnectedStart;
  uint32_t m_timeResolvingStart;
  uint32_t m_timeResolvedStart;
  uint32_t m_timeOpeningStart;
  uint32_t m_timeConnectedStart;
  uint32_t m_timeLastWrite;
};

#endif
