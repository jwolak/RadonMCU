/*-
 * BSD 3-Clause License
 *
 * No Copyrights 2026, Janusz Wolak
 * All rights not reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#ifndef EQUINIOS_H
#define EQUINIOS_H

#include <stdarg.h>
#include <stdint.h>

#include "EquiniosTypes.h"

/* Configure global log level. Messages with lower log level will be discarded. */
void log_set_level(log_level_t level);

/* Configure how often log_process flushes (every N calls). */
void log_set_process_every_n_calls(uint32_t calls);

/* Set timestamp provider used for log prefix, e.g. system tick counter. */
void log_set_timestamp_provider(uint32_t (*provider)(void));

/* Get count of log lines dropped due to ring buffer overflow. */
uint32_t log_get_dropped_lines(void);

/* Reset count of log lines dropped due to ring buffer overflow. */
void log_reset_dropped_lines(void);

/* Write a log message with the specified log level. */
void log_write(log_level_t level, const char *fmt, ...);

/* Call periodically from main loop or timer tick to flush queued logs. */
void log_process(void);

/* Convenience macros for different log levels. */
#define LOG_CRITICAL(fmt, ...) log_write(LOG_LEVEL_CRITICAL, "[CRITICAL] " fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) log_write(LOG_LEVEL_ERROR, "[ERROR] " fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) log_write(LOG_LEVEL_WARNING, "[WARNING] " fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) log_write(LOG_LEVEL_INFO, "[INFO] " fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) log_write(LOG_LEVEL_DEBUG, "[DEBUG] " fmt, ##__VA_ARGS__)
#define LOG_TRACE(fmt, ...) log_write(LOG_LEVEL_TRACE, "[TRACE] " fmt, ##__VA_ARGS__)

#endif
