#include "TempoDB.h"

void TempoDB::postSeriesUInt32(TempoDBSeries::Enum const i_series, uint32_t const i_value, uint32_t const i_timeStart_ms, uint32_t const i_budget_ms)
{
  char valueString[32];
  
  /***** JSONify *****/
  strcpy(valueString, "[{\"v\":");
  itoa(i_value, &valueString[strlen(valueString)], 10);
  strcat(valueString, "}]");

  /***** Log *****/
  TRIDENT_INFO(reinterpret_cast<__FlashStringHelper const *>(TEMPODB_SERIES_URIS[i_series]));
  
  /***** Post *****/
  if(m_connection.writePost(reinterpret_cast<__FlashStringHelper const *>(TEMPODB_SERIES_URIS[i_series]), valueString, i_timeStart_ms, i_budget_ms))
  {
    TRIDENT_INFO(valueString);
  }
}

void TempoDB::postSeriesDouble(TempoDBSeries::Enum const i_series, double const i_value, uint32_t const i_timeStart_ms, uint32_t const i_budget_ms)
{
  /***** JSONify *****/
  char valueString[64];
  strcpy(valueString, "[{\"v\":");
  dtostrf(i_value, 3, 2, &valueString[strlen(valueString)]);
  strcat(valueString, "}]");
  
  /***** Log *****/
  TRIDENT_INFO(reinterpret_cast<__FlashStringHelper const *>(TEMPODB_SERIES_URIS[i_series]));
  
  /***** Post *****/
  if(m_connection.writePost(reinterpret_cast<__FlashStringHelper const *>(TEMPODB_SERIES_URIS[i_series]), valueString, i_timeStart_ms, i_budget_ms))
  {
    TRIDENT_INFO(valueString);
  }
}
