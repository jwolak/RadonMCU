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

#include "EquiniosTypes.h"
#include "EquiniosLogger.h"

#include <stdio.h>
#include <stdarg.h>

#if defined(__GNUC__) || defined(__clang__)
#define EQUINIOS_UNUSED __attribute__((unused))
#else
#define EQUINIOS_UNUSED
#endif

#define EQUINIOS_LOG_MSG_MAX_LEN 256u

static log_level_t g_log_level = LOG_LEVEL_INFO;
static bool g_logger_initialized = false;
static uint32_t (*g_timestamp_provider)(void) = NULL;
static uint32_t g_timestamp_fallback = 0u;

static uint32_t get_timestamp(void)
{
  if (g_timestamp_provider != NULL)
  {
    return g_timestamp_provider();
  }

  return g_timestamp_fallback++;
}

static void ring_buffer_push_overwrite(struct RingBuffer *ring_buffer, uint8_t data)
{
  uint8_t dropped_byte;

  if (ring_buffer->is_full(ring_buffer))
  {
    (void)ring_buffer->pop(ring_buffer, &dropped_byte);
  }

  (void)ring_buffer->push(ring_buffer, data);
}

static void ring_buffer_write_string(struct RingBuffer *ring_buffer, const char *str)
{
  const unsigned char *ptr = (const unsigned char *)str;

  while (*ptr != '\0')
  {
    ring_buffer_push_overwrite(ring_buffer, *ptr);
    ptr++;
  }
}

static void logger_ensure_initialized(struct EquiniosLogger *this)
{
  if (!g_logger_initialized)
  {
    this->ring_buffer_ = RingBuffer.new();
    g_logger_initialized = true;
  }
}

static void set_log_level(struct EquiniosLogger *this EQUINIOS_UNUSED, log_level_t level)
{
  g_log_level = level;
}

static void set_timestamp_provider(struct EquiniosLogger *this EQUINIOS_UNUSED,
                                   uint32_t (*provider)(void))
{
  g_timestamp_provider = provider;
}

static void log_vwrite(struct EquiniosLogger *this, log_level_t level, const char *fmt,
                       va_list args)
{
  char prefixed_message[EQUINIOS_LOG_MSG_MAX_LEN];
  char message[EQUINIOS_LOG_MSG_MAX_LEN];
  uint32_t timestamp;

  if (level > g_log_level)
  {
    return;
  }

  logger_ensure_initialized(this);
  timestamp = get_timestamp();

  (void)vsnprintf(message, sizeof(message), fmt, args);
  (void)snprintf(prefixed_message, sizeof(prefixed_message), "[%lu] %s", (unsigned long)timestamp,
                 message);
  ring_buffer_write_string(&this->ring_buffer_, prefixed_message);
  ring_buffer_write_string(&this->ring_buffer_, "\r\n");
}

static void log_write(struct EquiniosLogger *this, log_level_t level, const char *fmt, ...)
{
  va_list args;

  va_start(args, fmt);
  this->log_vwrite(this, level, fmt, args);
  va_end(args);
}

static struct EquiniosLogger g_instance = {
    .set_log_level = set_log_level,
    .set_timestamp_provider = set_timestamp_provider,
    .log_vwrite = log_vwrite,
    .log_write = log_write,
};

static struct EquiniosLogger *instanceEquiniosLogger(void)
{
  logger_ensure_initialized(&g_instance);
  return &g_instance;
}

static struct EquiniosLogger newEquiniosLogger(void)
{
  return *instanceEquiniosLogger();
}

const struct EquiniosLoggerClass EquiniosLogger = {
    .instance = instanceEquiniosLogger,
    .new = newEquiniosLogger,
};
