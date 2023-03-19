#include "BufferedPrintResponse.h"

BufferedServerResponse::BufferedServerResponse(ESP8266WebServer *server) : _size(0)
{
  _server = server;
}

size_t BufferedServerResponse::write(uint8_t c)
{
  _buffer[_size++] = c;

  if (_size + 1 == 2000)
  {
    flush();
  }

  return _size;
}

void BufferedServerResponse::flush()
{
  _buffer[_size] = '\0';
  _server->sendContent(_buffer);
  _size = 0;
}