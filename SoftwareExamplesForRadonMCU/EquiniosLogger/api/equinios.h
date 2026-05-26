#ifndef EQUINIOS_H
#define EQUINIOS_H

#include <stdarg.h>

#include "EquiniosTypes.h"

void log_set_level(log_level_t level);

void log_write(log_level_t level, const char *fmt, ...);

#define LOGC(fmt, ...) log_write(LOG_LEVEL_CRITICAL, "[CRITICAL] " fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) log_write(LOG_LEVEL_ERROR, "[ERROR] " fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) log_write(LOG_LEVEL_WARNING, "[WARNING] " fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) log_write(LOG_LEVEL_INFO, "[INFO] " fmt, ##__VA_ARGS__)
#define LOGD(fmt, ...) log_write(LOG_LEVEL_DEBUG, "[DEBUG] " fmt, ##__VA_ARGS__)
#define LOGT(fmt, ...) log_write(LOG_LEVEL_TRACE, "[TRACE] " fmt, ##__VA_ARGS__)

#endif
