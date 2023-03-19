#include "Logger.h"

Logger::Logger(String name)
{
  _name = name;
  nameInserted = false;
}

void Logger::log(String message, bool noNewLine)
{
  if (noNewLine)
  {
    if (!nameInserted)
    {
      nameInserted = true;
      Serial.printf("[%s] %s", _name.c_str(), message.c_str());
    }
    else
    {
      Serial.print(message);
    }
  }
  else
  {
    if (nameInserted)
    {
      nameInserted = false;
      Serial.println();
    }
    Serial.printf("[%s] %s\n", _name.c_str(), message.c_str());
  }
}

void Logger::emptyLine()
{
  Serial.println();
}