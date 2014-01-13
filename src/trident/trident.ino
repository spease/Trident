#include "BMA250.h"
#include "SensorGPS.h"
#include "TempoDB.h"

#include <Adafruit_CC3000.h>
#include <Base64.h>  //For HTTPConnection.h
#include <ccspi.h>
#include <Adafruit_CC3000_Server.h>
#include <SoftwareSerial.h>  //For SensorGPS.h
#include <TinyGPS++.h>  //For SensorGPS.h
#include <SPI.h>
#include <Wire.h>  //Needed for BMA250.h

//#define TRIDENT_OUTPUT_NETWORK
#define TRIDENT_OUTPUT_SERIAL

static uint32_t const BMA_POST_PERIOD=1000;
static uint32_t const GPS_POST_PERIOD=1000;

static BMA250 gs_bma;
static uint32_t gs_bmaPost=0;

static SensorGPS gs_gps;
static uint32_t gs_gpsPost=0;

static NetworkConnection gs_network;
static TempoDB gs_tempoDB(gs_network);

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing BMA250...");
  gs_bma.setup();
  Serial.println("Initializing GPS...");
  gs_gps.setup();
  /*
  Serial.println("Initializing Network...");
  gs_network.setup();
  Serial.println("Initializing TempoDB...");
  gs_tempoDB.setup();
  */
  
  gs_bmaPost = millis()-BMA_POST_PERIOD;
  gs_gpsPost = millis()-GPS_POST_PERIOD;
}

void loop()
{
  gs_bma.update();
  gs_gps.update(100);
  
  if(timeSince_ms(gs_bmaPost) >= BMA_POST_PERIOD)
  {
#ifdef TRIDENT_OUTPUT_NETWORK
    gs_tempoDB.postSeriesUINT16(TempoDB::ACL_X, gs_bma.accelX(), 100);
    gs_tempoDB.postSeriesUINT16(TempoDB::ACL_Y, gs_bma.accelY(), 100);
    gs_tempoDB.postSeriesUINT16(TempoDB::ACL_Z, gs_bma.accelZ(), 100);
    gs_tempoDB.postSeriesDouble(TempoDB::ACL_TEMPERATURE, gs_bma.temperature(), 100);
#endif
#ifdef TRIDENT_OUTPUT_SERIAL
    Serial.println("ACL-X: "+String(gs_bma.accelX()));
    Serial.println("ACL-Y: "+String(gs_bma.accelY()));
    Serial.println("ACL-Z: "+String(gs_bma.accelZ()));
    Serial.println("ACL-Temperature: "+String((int)gs_bma.temperature()));
#endif
    gs_bmaPost = millis();
  }
  
  if(timeSince_ms(gs_gpsPost) >= GPS_POST_PERIOD)
  {
#ifdef TRIDENT_OUTPUT_NETWORK
    gs_tempoDB.postSeriesDouble(TempoDB::GPS_LATITUDE, gs_gps.data().location.lat(), 100);
    gs_tempoDB.postSeriesDouble(TempoDB::GPS_LONGITUDE, gs_gps.data().location.lng(), 100);
#endif
#ifdef TRIDENT_OUTPUT_SERIAL
    Serial.flush();
    Serial.println("GPS-Latitude: "+String((int)gs_gps.data().location.lat()));
    Serial.println("GPS-Longitude: "+String((int)gs_gps.data().location.lng()));
    Serial.println("GPS-Satellites: "+String(gs_gps.data().satellites.value()));
    Serial.println("GPS-Speed: "+String((int)gs_gps.data().speed.mps()));
    Serial.println("GPS-Course: "+String((int)gs_gps.data().course.deg()));
    Serial.println("GPS-Altitude: "+String((int)gs_gps.data().altitude.meters()));
    Serial.println("GPS-Date: "+String(gs_gps.data().date.year())+"-"+String(gs_gps.data().date.month())+"-"+String(gs_gps.data().date.day()));
    Serial.println("GPS-Time: "+String(gs_gps.data().time.hour())+"-"+String(gs_gps.data().time.minute())+"-"+String(gs_gps.data().time.second()));
#endif
    gs_gpsPost = millis();
  }
  
#ifdef TRIDENT_OUTPUT_NETWORK
  gs_network.update(100);
  gs_tempoDB.update(100);
#endif
}

