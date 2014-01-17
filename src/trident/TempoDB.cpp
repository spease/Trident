#include "TempoDB.h"

void TempoDB::postSeriesUINT16(TempoDB::Series const i_series, uint16_t const i_value, uint32_t const i_timeStart_ms, uint32_t const i_budget_ms)
{
  char valueString[32];
  strcpy(valueString, "[{\"v\":");
  itoa(i_value, &valueString[strlen(valueString)], 10);
  strcat(valueString, "}]");
  if(m_connection.writePost(TEMPODB_SERIES_URIS[i_series], valueString, i_timeStart_ms, i_budget_ms))
  {
    TRIDENT_INFO(valueString);
  }
}

void TempoDB::postSeriesDouble(TempoDB::Series const i_series, double const i_value, uint32_t const i_timeStart_ms, uint32_t const i_budget_ms)
{
  char valueString[64];
  strcpy(valueString, "[{\"v\":");
  dtostrf(i_value, 3, 2, &valueString[strlen(valueString)]);
  strcat(valueString, "}]");
  if(m_connection.writePost(reinterpret_cast<__FlashStringHelper const *>(TEMPODB_SERIES_URIS[i_series]), valueString, i_timeStart_ms, i_budget_ms))
  {
    TRIDENT_INFO(finalString);
  }
}
