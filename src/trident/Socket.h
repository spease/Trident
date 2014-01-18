#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "globals.h"

#include <Arduino.h>
#include <utility/socket.h>
#include <stddef.h>
#include <stdint.h>

class Socket
{
public:
  Socket();
  ~Socket();
  
  bool available() const;
  
  void close();
  bool connect(uint32_t const i_ip, uint16_t const i_port);
  
  bool connected() const;
  
  void print(__FlashStringHelper const * const i_string);
  void print(char const * const i_string);
  void println(char const * const i_string);
  void println(__FlashStringHelper const * const i_string);
  void println(size_t const i_integer);
  void println();
  
  uint8_t read();
private:
  int32_t m_socketFD;
};

#endif  /* _SOCKET_H_ */
