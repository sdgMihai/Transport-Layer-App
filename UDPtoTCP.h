#ifndef _UDPTOTCP_H
#define _UDPTOTCP_H 6

#include "helpers.h"

class __attribute__((packed)) UDPtoTCP{
 public:
  char topic[TOPIC_SIZE];
  char data_type;
  uint32_t payloadSize;
  struct sockaddr_in addr;
  char payload[BUFLEN - TOPIC_SIZE - sizeof(data_type)];
  
  char endOfMsg;

  UDPtoTCP() ;
  
  UDPtoTCP(const UDPtoTCP&) ;

  int getInt() const;

  void setInt(int) ;

  double getShortReal() const;

  void setShortReal(double) ;

  double getFloat() const;

  void setFloat(double) ;

  const char* getString() const;

  void setString(const char*) ;

};

#endif
