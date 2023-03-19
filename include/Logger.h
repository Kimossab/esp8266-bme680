#ifndef logger_h__
#define logger_h__

#include <Arduino.h>

class Logger
{
public:
  Logger(String name);
  void log(String message, bool noNewLine = false);
  void emptyLine();

private:
  String _name;
  bool nameInserted;
};

#endif