// https://github.com/bblanchon/ArduinoJson/issues/166#issuecomment-158915757

#ifndef BUFFERED_PRINT_RESPONSE_H__
#define BUFFERED_PRINT_RESPONSE_H__

#include <Print.h>
#include <ESP8266WebServer.h>

#define MAX_PRINT_BUFFER_SIZE 2000

class BufferedServerResponse : public Print
{
public:
  BufferedServerResponse(ESP8266WebServer *server);
  virtual size_t write(uint8_t c);
  void flush();

private:
  ESP8266WebServer *_server;
  size_t _size;
  char _buffer[MAX_PRINT_BUFFER_SIZE];
};

#endif