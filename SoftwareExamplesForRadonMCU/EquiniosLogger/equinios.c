#include "equinios.h"
#include "EquiniosLogger.h"

#include <stdarg.h>

void log_set_level(log_level_t level)
{
  struct EquiniosLogger *logger = EquiniosLogger.instance();
  logger->set_log_level(logger, level);
}

void log_write(log_level_t level, const char *fmt, ...)
{
  struct EquiniosLogger *logger = EquiniosLogger.instance();
  va_list args;

  va_start(args, fmt);
  logger->log_vwrite(logger, level, fmt, args);
  va_end(args);
}