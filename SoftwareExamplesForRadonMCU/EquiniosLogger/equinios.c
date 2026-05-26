#include "equinios.h"

#include <stdio.h>
#include <stdarg.h>

static log_level_t g_log_level = LOG_LEVEL_INFO;

void log_set_level(log_level_t level)
{
  g_log_level = level;
}

void log_write(log_level_t level, const char *fmt, ...)
{
  va_list args;

  if (level > g_log_level)
  {
    return;
  }

  va_start(args, fmt);
  vprintf(fmt, args);
  printf("\r\n");
  va_end(args);
}