#ifndef EQUINIOS_H
#define EQUINIOS_H

#include <stdarg.h>

typedef enum
{
  LOG_LEVEL_ERROR = 0,
  LOG_LEVEL_WARN = 1,
  LOG_LEVEL_INFO = 2,
  LOG_LEVEL_DEBUG = 3
} log_level_t;

void log_set_level(log_level_t level);

void log_write(log_level_t level, const char *fmt, ...);

#define LOGE(fmt, ...) log_write(LOG_LEVEL_ERROR, "[ERR] " fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) log_write(LOG_LEVEL_WARN, "[WRN] " fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) log_write(LOG_LEVEL_INFO, "[INF] " fmt, ##__VA_ARGS__)
#define LOGD(fmt, ...) log_write(LOG_LEVEL_DEBUG, "[DBG] " fmt, ##__VA_ARGS__)

#endif