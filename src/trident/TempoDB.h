#ifndef _TEMPODB_H_
#define _TEMPODB_H_

#include "HTTPConnection.h"
#include "NetworkConnection.h"

class TempoDB
{
public:
  enum Series
  {
    GPS_LATITUDE=0,
    GPS_LONGITUDE,
    GPS_SATELLITES,
    ACL_X,
    ACL_Y,
    ACL_Z,
    ACL_TEMPERATURE,
    TMP_TEMPERATURE,
    HMD_HUMIDITY
  };
  static char const * const msc_seriesKey[];
  
  TempoDB(NetworkConnection &i_network)
  :m_connection(msc_apiServer, i_network)
  {
  }
  
  void setup()
  {
    m_connection.setup();
    m_connection.setAuthenticationBasic(msc_auth);
  }
  
  void update(uint32_t const i_budget_ms)
  {
    m_connection.update(i_budget_ms);
  }
  
  void postSeriesUINT16(Series const i_series, uint16_t const i_value, uint32_t const i_budget_ms)
  {
    char uriString[128];
    char valueString[64];
    snprintf(valueString, sizeof(valueString)-1, "[{\"v\":%d}]", i_value);
    snprintf(uriString, sizeof(uriString)-1, "/%s/series/key/%s/data/", msc_apiVersion, msc_seriesKey[i_series]);
    m_connection.writePost(uriString, valueString, i_budget_ms);
  }
  
  void postSeriesDouble(Series const i_series, double const i_value, uint32_t const i_budget_ms)
  {
    char uriString[128];
    char valueString[64];
    snprintf(valueString, sizeof(valueString)-1, "[{\"v\":%2.6f}]", i_value);
    snprintf(uriString, sizeof(uriString)-1, "/%s/series/key/%s/data/", msc_apiVersion, msc_seriesKey[i_series]);
    m_connection.writePost(uriString, valueString, i_budget_ms);
  }
  
private:
  static char const * const msc_apiServer;
  static char const * const msc_apiVersion;
  static char const * const msc_auth;
  /*
  static char const * const msc_sensorID[] = {
    "606904c78caa4fac89723293075951a6",
    "b658306da1e04da7b269fedfd99dcf8f",
    "3d91bf719ac944e395a06a6cd7ef11f9",
    "32082e08691b4a2f8ed245b17078921c"
  };
  */
  
  HTTPConnection m_connection;
};

char const * const TempoDB::msc_apiServer="api.tempo-db.com";
char const * const TempoDB::msc_apiVersion="v1";
char const * const TempoDB::msc_auth="048cee316ae74274bb4b2f7730eba236:25f9d7663f7147a2a5cf0b5635f0a28a";
char const * const TempoDB::msc_seriesKey[] = {
    "device:1.GPS.Latitude.",
    "device:1.GPS.Longitude.",
    "device:1.GPS.Satellites.",
    "device:1.ACL.AccelerationX.",
    "device:1.ACL.AccelerationY.",
    "device:1.ACL.AccelerationZ.",
    "device:1.ACL.Temperature.",
    "device:1.TMP.Temperature",
    "device:1.HMD.Humidity",
  };

#endif /* _TEMPODB_H_ */
