#include "NetworkConnection.h"

bool NetworkConnection::acquire(uint32_t const i_timeStart_ms, uint32_t const i_duration_ms)
{ 
  while(timeSince_ms(i_timeStart_ms) < i_duration_ms)
  {
    if(m_status == STATUS_DISCONNECTED)
    {
      /***** Begin Scanning *****/
      m_cc.scanSSIDs(NETWORKCONNECTION_timeout_ms);
      m_timeLastActivity = millis();
      m_status = STATUS_SCANNING;
      TRIDENT_INFO(F("NW Scan"));
    }
    else if(m_status == STATUS_SCANNING)
    {
      uint8_t apRSSI=0;
      uint8_t apSecurity=0;
      char apName[33]={'\0'};
      uint8_t apValid=false;
      
      while(timeSince_ms(i_timeStart_ms) < i_duration_ms)
      {
        apValid = m_cc.getNextSSID(&apRSSI, &apSecurity, apName) != 0;
        if(apValid != 0)
        {
          TRIDENT_INFO(apName);
          //TRIDENT_INFO("NW AP:"+String(apName)/*+"' / RSSI: "+String(apRSSI)+" / SEC: "+String(apSecurity)*/);
          if(apSecurity == WLAN_SEC_UNSEC)
          {
            m_cc.scanSSIDs(0);
            if(m_cc.connectOpen(apName))
            {
              m_status = STATUS_ASSOCIATING;
              m_timeLastActivity = millis();
              TRIDENT_INFO(String(F("NW Assoc:"))+String(apName));
              break;
            }
            else
            {
              m_status = STATUS_DISCONNECTED;
              TRIDENT_INFO(F("NW Abort"));
            }
          }
        }
        else if(timeSince_ms(m_timeLastActivity) > NETWORKCONNECTION_timeout_ms)
        {
          this->lost();
        }
      }
      
      return false;
    }
    else if(m_status == STATUS_ASSOCIATING)
    {
      if(m_cc.checkConnected())
      {
        m_status = STATUS_REQUESTING;
        m_timeLastActivity = millis();
        TRIDENT_INFO(F("NW Req"));
      }
      else if(timeSince_ms(m_timeLastActivity) > NETWORKCONNECTION_timeout_ms)
      {
        this->lost();
      }
    }
    else if(m_status == STATUS_REQUESTING)
    {
      if(m_cc.checkDHCP())
      {
        m_status = STATUS_CONNECTED;
        TRIDENT_INFO(F("NW Acp"));
      }
      else if(timeSince_ms(m_timeLastActivity) > NETWORKCONNECTION_timeout_ms)
      {
        this->lost();
      }
    }
    else if(m_status == STATUS_CONNECTED)
    {
      if(!m_cc.checkConnected())
      {
        this->lost();
      }
      return true;
    }
  }
}

void NetworkConnection::lost()
{
  m_cc.disconnect();
  m_status = STATUS_DISCONNECTED;
  TRIDENT_INFO(F("NW lost"));
}
