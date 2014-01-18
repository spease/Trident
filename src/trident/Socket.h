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
  void println(char const * const i_string);
  void println(__FlashStringHelper const * const i_string);
  void println(size_t const i_integer);
  void println();
  
  int32_t read(char *i_buffer=NULL, size_t const i_bufferLength=0);
private:
  int32_t m_socketFD;
};

#endif  /* _SOCKET_H_ */
