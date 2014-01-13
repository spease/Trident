#ifndef _NETWORKCONNECTION_H_
#define _NETWORKCONNECTION_H_
#include <Adafruit_CC3000.h>

class NetworkConnection
{
public:
  NetworkConnection()
  :m_cc(msc_pinCS, msc_pinIRQ, msc_pinVBAT, SPI_CLOCK_DIV2),m_status(STATUS_DISCONNECTED)
  {}
  
  bool acquire(uint32_t const i_duration_ms)
  {
    uint32_t timeStart = millis();
    
    while(m_status != STATUS_CONNECTED)
    {
      /***** Begin Scanning *****/
      if(m_status == STATUS_DISCONNECTED)
      {
        m_scanIndex = m_cc.startSSIDscan();
        m_status = STATUS_SCANNING;
        m_timeScanningStart = millis();
        Serial.println("WLN: Scanning for APs...");
      }
      else if(m_status == STATUS_SCANNING)
      {
        uint8_t apRSSI=0;
        uint8_t apSecurity=0;
        char apName[32]={'\0'};
        bool apValid=false;
        
        while(m_scanIndex != 0)
        {
          --m_scanIndex;
          
          apValid = m_cc.getNextSSID(&apRSSI, &apSecurity, apName);
          Serial.println("WLN: Detected '"+String(apName)+"' / RSSI: "+String(apRSSI)+" / SEC: "+String(apSecurity));
          if(apValid && apSecurity == WLAN_SEC_UNSEC)
          {
            if(m_cc.connectOpen(apName))
            {
              m_status = STATUS_ASSOCIATING;
              m_timeAssociatingStart = millis();
              Serial.println("WLN: Associating with AP '"+String(apName)+"'...");
              break;
            }
          }
          
          /***** Check time *****/
          if(timeSince_ms(timeStart) > i_duration_ms)
          {
            return false;
          }
        }
      }
      else if(m_status == STATUS_ASSOCIATING)
      {
        if(m_cc.checkConnected())
        {
          m_status = STATUS_REQUESTING;
          m_timeRequestingStart = millis();
          Serial.println("WLN: Requesting DHCP...");
        }
      }
      else if(m_status == STATUS_REQUESTING)
      {
        if(m_cc.checkDHCP())
        {
          m_status = STATUS_CONNECTED;
          m_timeConnectionStart = millis();
          Serial.println("WLN: Connected.");
        }
        else if(timeSince_ms(m_timeRequestingStart) > msc_timeout_ms)
        {
          this->lost();
        }
      }
      
      /***** Check time *****/
      if(timeSince_ms(timeStart) > i_duration_ms)
      {
        return false;
      }
    }
    
    return true;
  }
  
  Adafruit_CC3000 cc()
  {
    return m_cc;
  }
  
  void lost()
  {
    m_cc.disconnect();
    m_status = STATUS_DISCONNECTED;
    m_timeDisconnectedStart = millis();
    Serial.println("WLN: Connection lost.");
  }
  
  void setup()
  {
    pinMode(msc_pinIRQ, INPUT);
    pinMode(msc_pinVBAT, OUTPUT);
    digitalWrite(msc_pinVBAT, HIGH);
    pinMode(msc_pinCS, INPUT);
    
    this->acquire(msc_timeout_ms);
  }
  
  void update(uint32_t const i_budget_ms)
  {
    this->acquire(i_budget_ms);
  }
private:
  static uint32_t const msc_timeout_ms=5000;
  static int const msc_pinIRQ=2;
  static int const msc_pinVBAT=A3;
  static int const msc_pinCS=8;
  
  enum Status
  {
    STATUS_DISCONNECTED=0,
    STATUS_SCANNING=1,
    STATUS_ASSOCIATING=2,
    STATUS_REQUESTING=3,
    STATUS_CONNECTED=4
  };
  
  Adafruit_CC3000 m_cc;
  uint8_t m_scanIndex;
  NetworkConnection::Status m_status;
  uint32_t m_timeDisconnectedStart;
  uint32_t m_timeScanningStart;
  uint32_t m_timeAssociatingStart;
  uint32_t m_timeRequestingStart;
  uint32_t m_timeConnectionStart;
};

#endif  /* _NETWORKCONNECTION_H_ */
