#ifndef _TEMPODB_H_
#define _TEMPODB_H_

#include "globals.h"
#include "HTTPConnection.h"
#include "NetworkConnection.h"

TEMPODB_SERIES_KEY(GPS_LATITUDE, "GPS.Latitude.");
TEMPODB_SERIES_KEY(GPS_LONGITUDE, "GPS.Longitude.");
TEMPODB_SERIES_KEY(GPS_SATELLITES, "GPS.Satellites.");
TEMPODB_SERIES_KEY(ACL_X, "ACL.AccelerationX.");
TEMPODB_SERIES_KEY(ACL_Y, "ACL.AccelerationY.");
TEMPODB_SERIES_KEY(ACL_Z, "ACL.AccelerationZ.");
TEMPODB_SERIES_KEY(ACL_TEMPERATURE, "ACL.Temperature.");
TEMPODB_SERIES_KEY(TMP_TEMPERATURE, "TMP.Temperature.");
TEMPODB_SERIES_KEY(HMD_HUMIDITY, "HMD.Humidity.");

static __FlashStringHelper const * const TEMPODB_SERIES_URIS[] = {
  TEMPODB_SERIES_URI(GPS_LATITUDE),
  TEMPODB_SERIES_URI(GPS_LONGITUDE),
  TEMPODB_SERIES_URI(GPS_SATELLITES),
  TEMPODB_SERIES_URI(ACL_X),
  TEMPODB_SERIES_URI(ACL_Y),
  TEMPODB_SERIES_URI(ACL_Z),
  TEMPODB_SERIES_URI(ACL_TEMPERATURE),
  TEMPODB_SERIES_URI(TMP_TEMPERATURE),
  TEMPODB_SERIES_URI(HMD_HUMIDITY),
};

  /*
  static char const * const msc_sensorID[] = {
    "606904c78caa4fac89723293075951a6",
    "b658306da1e04da7b269fedfd99dcf8f",
    "3d91bf719ac944e395a06a6cd7ef11f9",
    "32082e08691b4a2f8ed245b17078921c"
  };
  */

class TempoDB
{
public:
  enum Series
  {
    GPS_LATITUDE=0,
    GPS_LONGITUDE=1,
    GPS_SATELLITES=2,
    ACL_X=3,
    ACL_Y=4,
    ACL_Z=5,
    ACL_TEMPERATURE=6,
    TMP_TEMPERATURE=7,
    HMD_HUMIDITY=8
  };
  
  TempoDB()
  :m_connection(TRIDENT_TEMPODB_API_SERVER, reinterpret_cast<__FlashStringHelper const *>(TRIDENT_TEMPODB_API_KEY_BASE64))
  {
  }
  
  void setup()
  {
    m_connection.setup();
    
    TRIDENT_INFO("TDB");
  }
  
  void update(uint32_t const i_timeStart_ms, uint32_t const i_budget_ms)
  {
    m_connection.update(i_timeStart_ms, i_budget_ms);
  }
  
  void postSeriesUINT16(Series const i_series, uint16_t const i_value, uint32_t const i_timeStart_ms, uint32_t const i_budget_ms);
  void postSeriesDouble(Series const i_series, double const i_value, uint32_t const i_timeStart_ms, uint32_t const i_budget_ms);
  
private:
  HTTPConnection m_connection;
};

#endif /* _TEMPODB_H_ */
