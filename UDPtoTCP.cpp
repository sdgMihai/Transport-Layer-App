#include "UDPtoTCP.h"
#include "cstdio"

UDPtoTCP::UDPtoTCP() {
  memset(&topic, 0, BUFLEN + sizeof(int) + sizeof(struct sockaddr_in));
}

UDPtoTCP::UDPtoTCP(const UDPtoTCP& cp) {
  memset(&topic, 0, BUFLEN + sizeof(int) + sizeof(struct sockaddr_in));
  strcpy(this->topic, cp.topic);
  this->data_type = cp.data_type;
  this->payloadSize = cp.payloadSize;
  memcpy(&(this->addr), &cp.addr, sizeof(cp.addr));
  memcpy(this->payload, cp.payload, payloadSize);
}

int UDPtoTCP::getInt() const{
  int res;
  sscanf(payload, "%d", &res);
  return res;
}

void UDPtoTCP::setInt(int value) {
   sprintf(payload, "%d", value);
   payloadSize = strlen(payload);
}

double UDPtoTCP::getShortReal() const{
  double res;
  sscanf(payload, "%lf", &res);
  return res;
}

void UDPtoTCP::setShortReal(double value) {
  sprintf(payload,"%f", value);
  payloadSize = strlen(payload);
}


double UDPtoTCP::getFloat() const{
  double res;
  sscanf(payload, "%lf", &res);
  return res;
}

void UDPtoTCP::setFloat(double value) {
  sprintf(payload,"%f", value);
  payloadSize = strlen(payload);
}

const char* UDPtoTCP::getString() const{
  return payload;
}

void UDPtoTCP::setString(const char* value) {
  strcpy(payload, value);
  payloadSize = strlen(payload);
}
