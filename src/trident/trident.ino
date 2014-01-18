#include "globals.h"

#ifdef TRIDENT_ACL
#include "BMA250.h"
#endif
#ifdef TRIDENT_GPS
#include "SensorGPS.h"
#endif
#if defined(TRIDENT_HMD) || defined(TRIDENT_TMP)
#include "SensorTMPHMD.h"
#include <Sensirion.h>
#endif
#ifdef TRIDENT_OUTPUT_NETWORK
#include "TempoDB.h"
#endif

#ifdef TRIDENT_OUTPUT_NETWORK
#include <Adafruit_CC3000.h>         //For NetworkConnection.h
#include <Adafruit_CC3000_Server.h>  //For NetworkConnection.h
//#include <Base64.h>                  //For HTTPConnection.h
#include <SPI.h>
#include <ccspi.h>
#endif

#ifdef TRIDENT_GPS
#include <TinyGPS++.h>               //For SensorGPS.h
#include <SoftwareSerial.h>          //For SensorGPS.h
#endif
#ifdef TRIDENT_ACL
#include <Wire.h>                    //For BMA250.h
#endif


#ifdef TRIDENT_ACL
static BMA250<> gs_bma;
static uint32_t gs_bmaPost=0;
#endif

#ifdef TRIDENT_GPS
static SensorGPS gs_gps;
static uint32_t gs_gpsPost=0;
#endif

#if defined(TRIDENT_HMD) || defined(TRIDENT_TMP)
static SensorTMPHMD gs_tmphmd;
static uint32_t gs_tmphmdPost=0;
#endif

#ifdef TRIDENT_OUTPUT_NETWORK
static TempoDB gs_tempoDB;
#endif

void setup()
{
#if defined(TRIDENT_OUTPUT_SERIAL) || defined(TRIDENT_LOG_SERIAL)
  Serial.begin(115200);
#endif
#ifdef TRIDENT_ACL
  gs_bma.setup();
  gs_bmaPost = millis()-TRIDENT_ACL_POST_PERIOD_MS;
#endif
#ifdef TRIDENT_GPS
  gs_gps.setup();
  gs_gpsPost = millis()-TRIDENT_GPS_POST_PERIOD_MS;
#endif
#if defined(TRIDENT_HMD) || defined(TRIDENT_TMP)
  gs_tmphmdPost = millis()-TRIDENT_TMPHMD_POST_PERIOD_MS;
#endif
#ifdef TRIDENT_OUTPUT_NETWORK
  gs_tempoDB.setup();
#endif

  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  OCR1A = 15624;
  TCCR1B |= (1<<WGM12);
  TCCR1B |= (1<<CS12)|(1<<CS10);
  TIMSK1 |= (1<<OCIE1A);
  sei();
}

void loop()
{
#ifdef TRIDENT_OUTPUT_NETWORK
  gs_tempoDB.update(millis(), 100);
#endif
  
#ifdef TRIDENT_ACL
  if(timeSince_ms(gs_bmaPost) >= TRIDENT_ACL_POST_PERIOD_MS)
  {
#ifdef TRIDENT_OUTPUT_NETWORK
    gs_tempoDB.postSeriesUInt32(TempoDBSeries::ACL_X, gs_bma.accelX(), millis(), 100);
    gs_tempoDB.postSeriesUInt32(TempoDBSeries::ACL_Y, gs_bma.accelY(), millis(), 100);
    gs_tempoDB.postSeriesUInt32(TempoDBSeries::ACL_Z, gs_bma.accelZ(), millis(), 100);
    gs_tempoDB.postSeriesDouble(TempoDBSeries::ACL_TEMPERATURE, gs_bma.temperature(), millis(), 100);
#endif
#ifdef TRIDENT_OUTPUT_SERIAL
    TRIDENT_POST("ACL-X: "+String(gs_bma.accelX()));
    TRIDENT_POST("ACL-Y: "+String(gs_bma.accelY()));
    TRIDENT_POST("ACL-Z: "+String(gs_bma.accelZ()));
    TRIDENT_POST("ACL-Temperature: "+String((int)gs_bma.temperature()));
#endif
    gs_bmaPost = millis();
  }
#endif
  
#ifdef TRIDENT_GPS
  if(timeSince_ms(gs_gpsPost) >= TRIDENT_GPS_POST_PERIOD_MS)
  {
#ifdef TRIDENT_OUTPUT_NETWORK
    gs_tempoDB.postSeriesDouble(TempoDBSeries::GPS_LATITUDE, gs_gps.data().location.lat(), millis(), 100);
    gs_tempoDB.postSeriesDouble(TempoDBSeries::GPS_LONGITUDE, gs_gps.data().location.lng(), millis(), 100);
    gs_tempoDB.postSeriesUInt32(TempoDBSeries::GPS_SATELLITES, gs_gps.data().satellites.value(), millis(), 100);
    gs_tempoDB.postSeriesDouble(TempoDBSeries::GPS_SPEED, gs_gps.data().speed.mps(), millis(), 100);
    gs_tempoDB.postSeriesDouble(TempoDBSeries::GPS_COURSE, gs_gps.data().course.deg(), millis(), 100);
    gs_tempoDB.postSeriesDouble(TempoDBSeries::GPS_ALTITUDE, gs_gps.data().altitude.meters(), millis(), 100);
#endif
#ifdef TRIDENT_OUTPUT_SERIAL
    TRIDENT_POST("GPS-Latitude: "+String((int)gs_gps.data().location.lat()));
    TRIDENT_POST("GPS-Longitude: "+String((int)gs_gps.data().location.lng()));
    TRIDENT_POST("GPS-Satellites: "+String(gs_gps.data().satellites.value()));
    TRIDENT_POST("GPS-Speed: "+String((int)gs_gps.data().speed.mps()));
    TRIDENT_POST("GPS-Course: "+String((int)gs_gps.data().course.deg()));
    TRIDENT_POST("GPS-Altitude: "+String((int)gs_gps.data().altitude.meters()));
    TRIDENT_POST("GPS-Date: "+String(gs_gps.data().date.year())+"-"+String(gs_gps.data().date.month())+"-"+String(gs_gps.data().date.day()));
    TRIDENT_POST("GPS-Time: "+String(gs_gps.data().time.hour())+"-"+String(gs_gps.data().time.minute())+"-"+String(gs_gps.data().time.second()));
#endif
    gs_gpsPost = millis();
  }
#endif

#ifdef TRIDENT_TMP
  if(timeSince_ms(gs_tmphmdPost) >= TRIDENT_TMPHMD_POST_PERIOD_MS)
  {
#ifdef TRIDENT_OUTPUT_NETWORK
#ifdef TRIDENT_HMD
    gs_tempoDB.postSeriesDouble(TempoDBSeries::HMD_HUMIDITY, gs_tmphmd.humidity(), millis(), 100);
#endif
#ifdef TRIDENT_TMP
    gs_tempoDB.postSeriesDouble(TempoDBSeries::TMP_TEMPERATURE, gs_tmphmd.temperature(), millis(), 100);
#endif
#endif
#ifdef TRIDENT_OUTPUT_SERIAL
#ifdef TRIDENT_HMD
    Serial.println("HMD: "+String(gs_tmphmd.humidity()));
#endif
#ifdef TRIDENT_TMP
    Serial.println("TMP: "+String(gs_tmphmd.temperature()));
#endif
#endif
    gs_tmphmdPost = millis();
  }
#endif
}

ISR(TIMER1_COMPA_vect)
{
  sei();
#ifdef TRIDENT_ACL
  gs_bma.update();
#endif
#ifdef TRIDENT_GPS
  gs_gps.update(millis(), 100);
#endif
#if defined(TRIDENT_TMP) || defined(TRIDENT_HMD)
  gs_tmphmd.update();
#endif
}
