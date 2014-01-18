#include "Socket.h"

#include "utility/cc3000_common.h"

Socket::Socket()
:m_socketFD(-1)
{
}

Socket::~Socket()
{
  if(m_socketFD != -1)
  {
    this->close();
  }
}

bool Socket::available() const
{
  timeval timeout;
  fd_set fdArray = {0};
  FD_SET(m_socketFD, &fdArray);
  
  timeout.tv_sec = 0;
  timeout.tv_usec = 5000;
  
  int16_t selectResult = select(m_socketFD+1, &fdArray, NULL, NULL, &timeout);
  if(selectResult != 1)
  {
    return false;
  }
  
  return true;
}

void Socket::close()
{
  closesocket(m_socketFD);
  m_socketFD = -1;
}

bool Socket::connect(uint32_t const i_ip, uint16_t const i_port)
{
  sockaddr socketAddress = {0};
  int32_t socketFD = -1;
  int32_t socketResult = -1;
  
  /***** Open *****/
  socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(socketFD < 0)
  {
    TRIDENT_ERROR(F("S fl"));
    return false;
  }
  
  /***** Set non-blocking *****/
  /*
  socketResult = setsockopt(socketFD, SOL_SOCKET, SOCKOPT_RECV_NONBLOCK, SOCK_ON, sizeof(SOCK_ON));
  if(socketResult != 0)
  {
    TRIDENT_ERROR(F("S blk"));
    return false;
  }*/
  
  /***** Build Address *****/
  socketAddress.sa_family = AF_INET;
  socketAddress.sa_data[0] = (i_port & 0xFF00) >> 8;
  socketAddress.sa_data[1] = (i_port & 0x00FF);
  socketAddress.sa_data[2] = i_ip >> 24;
  socketAddress.sa_data[3] = i_ip >> 16;
  socketAddress.sa_data[4] = i_ip >> 8;
  socketAddress.sa_data[5] = i_ip;
  
  /***** Connect *****/
  socketResult = ::connect(socketFD, &socketAddress, sizeof(socketAddress));
  if(socketResult < 0)
  {
    closesocket(socketFD);
    return false;
  }
  
  /***** Save *****/
  closesocket(m_socketFD);
  m_socketFD = socketFD;
  
  TRIDENT_INFO(F("S cnt"));
  return true;
}

bool Socket::connected() const
{
  return m_socketFD != -1;
}

void Socket::print(__FlashStringHelper const * const i_string)
{
  static size_t const txBufferLengthMax = 32;
  char txBuffer[txBufferLengthMax];
  size_t txBufferLength = 0;
  size_t stringPosition = 0;
  
  if(i_string == NULL)
  {
    return;
  }
  
  do
  {
    while(txBufferLength < txBufferLengthMax)
    {
      txBuffer[txBufferLength] = pgm_read_byte(&reinterpret_cast<char const PROGMEM *>(i_string)[stringPosition]);
      ++stringPosition;
      if(txBuffer[txBufferLength] == '\0')
      {
        stringPosition = 0;
        break;
      }
      ++txBufferLength;
    }
    
    if(txBufferLength > 0)
    {
     ::send(m_socketFD, txBuffer, txBufferLength, 0);
      txBufferLength = 0;
    }
  } while(stringPosition != 0);
}

void Socket::println()
{
  ::send(m_socketFD, "\r\n", 2, 0);
}

void Socket::print(char const * const i_string)
{
  size_t stringLength = 0;
  if(i_string == NULL)
  {
    return;
  }
  
  stringLength = strlen(i_string);
  ::send(m_socketFD, i_string, stringLength, 0);
}

void Socket::println(char const * const i_string)
{
  this->print(i_string);
  ::send(m_socketFD, "\r\n", 2, 0);
}

void Socket::println(__FlashStringHelper const * const i_string)
{
  this->print(i_string);
  ::send(m_socketFD, "\r\n", 2, 0);
}

void Socket::println(size_t const i_integer)
{
  char txBuffer[32];
  
  /***** Generate integer *****/
  itoa(i_integer, txBuffer, 10);
  
  /***** Send it *****/
  this->print(txBuffer);
  ::send(m_socketFD, "\r\n", 2, 0);
}

uint8_t Socket::read()
{
  uint8_t buffer[50]={0};
  int16_t recvResult=recv(m_socketFD, &buffer, 50, 0);
  if(recvResult == -57)
  {
    this->close();
    return buffer[0];
  }
  
  return buffer[0];
}
