#include "equinios.h"
#include "EquiniosLogger.h"

#include <stdarg.h>
#include <stdio.h>

void log_set_level(log_level_t level)
{
  struct EquiniosLogger *logger = EquiniosLogger.instance();
  logger->set_log_level(logger, level);
}

void log_write(log_level_t level, const char *fmt, ...)
{
  struct EquiniosLogger *logger = EquiniosLogger.instance();
  va_list args;
  char message[128];
  int written;

  va_start(args, fmt);
  written = vsnprintf(message, sizeof(message), fmt, args);
  va_end(args);

  if (written < 0)
  {
    return;
  }

  message[sizeof(message) - 1U] = '\0';
  logger->log_write(logger, level, "%s", message);
}