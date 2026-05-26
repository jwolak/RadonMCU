#ifndef EQUINIOS_H
#define EQUINIOS_H

#include <stdarg.h>

typedef enum
{
  LOG_LEVEL_CRITICAL = 0,
  LOG_LEVEL_ERROR = 1,
  LOG_LEVEL_WARNING = 2,
  LOG_LEVEL_INFO = 3,
  LOG_LEVEL_DEBUG = 4,
  LOG_LEVEL_TRACE = 5,
} log_level_t;

void log_set_level(log_level_t level);

void log_write(log_level_t level, const char *fmt, ...);

#define LOGC(fmt, ...) log_write(LOG_LEVEL_CRITICAL, "[CRITICAL] " fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) log_write(LOG_LEVEL_ERROR, "[ERROR] " fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) log_write(LOG_LEVEL_WARNING, "[WARNING] " fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) log_write(LOG_LEVEL_INFO, "[INFO] " fmt, ##__VA_ARGS__)
#define LOGD(fmt, ...) log_write(LOG_LEVEL_DEBUG, "[DEBUG] " fmt, ##__VA_ARGS__)
#define LOGT(fmt, ...) log_write(LOG_LEVEL_TRACE, "[TRACE] " fmt, ##__VA_ARGS__)

#endif