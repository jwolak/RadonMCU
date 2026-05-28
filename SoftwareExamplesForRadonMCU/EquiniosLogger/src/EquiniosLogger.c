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
#include <string.h>

#if defined(__GNUC__) || defined(__clang__)
#define EQUINIOS_UNUSED __attribute__((unused))
#else
#define EQUINIOS_UNUSED
#endif

#define EQUINIOS_LOG_MSG_MAX_LEN 256u

static void ring_buffer_discard_oldest_line(struct RingBuffer *ring_buffer)
{
  uint8_t byte;

  while (ring_buffer->pop(ring_buffer, &byte))
  {
    if (byte == '\n')
    {
      break;
    }
  }
}

static void logger_enqueue_line(struct EquiniosLogger *this, const char *line)
{
  size_t line_len = strlen(line);
  size_t free_space;
  size_t i;

  if (line_len > (size_t)RING_BUFFER_SIZE)
  {
    line = line + (line_len - (size_t)RING_BUFFER_SIZE);
    line_len = (size_t)RING_BUFFER_SIZE;
  }

  free_space = (size_t)RING_BUFFER_SIZE - this->ring_buffer_.size(&this->ring_buffer_);
  while (free_space < line_len)
  {
    if (this->ring_buffer_.is_empty(&this->ring_buffer_))
    {
      break;
    }

    ring_buffer_discard_oldest_line(&this->ring_buffer_);
    free_space = (size_t)RING_BUFFER_SIZE - this->ring_buffer_.size(&this->ring_buffer_);
  }

  for (i = 0; i < line_len; i++)
  {
    if (!this->ring_buffer_.push(&this->ring_buffer_, (uint8_t)line[i]))
    {
      break;
    }
  }
}

static void logger_ensure_initialized(struct EquiniosLogger *this)
{
  if (!this->initialized_)
  {
    this->ring_buffer_ = RingBuffer.new();
    this->initialized_ = true;
  }
}

/* public methods */
static void set_log_level(struct EquiniosLogger *this, log_level_t level)
{
  this->log_level_ = level;
}

static void set_timestamp_provider(struct EquiniosLogger *this, uint32_t (*provider)(void))
{
  this->timestamp_provider_.set_provider(&this->timestamp_provider_, provider);
}

static void log_vwrite(struct EquiniosLogger *this, log_level_t level, const char *fmt,
                       va_list args)
{
  char line[EQUINIOS_LOG_MSG_MAX_LEN + 4u];
  char message[EQUINIOS_LOG_MSG_MAX_LEN];
  uint32_t timestamp;

  if (level > this->log_level_)
  {
    return;
  }

  logger_ensure_initialized(this);
  timestamp = this->timestamp_provider_.get_timestamp(&this->timestamp_provider_);

  (void)vsnprintf(message, sizeof(message), fmt, args);
  (void)snprintf(line, sizeof(line), "[%lu] %s\r\n", (unsigned long)timestamp, message);
  logger_enqueue_line(this, line);
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
    .initialized_ = false,
    .log_level_ = LOG_LEVEL_INFO,
};

static struct EquiniosLogger *instanceEquiniosLogger(void)
{
  logger_ensure_initialized(&g_instance);
  return &g_instance;
}

static struct EquiniosLogger newEquiniosLogger(void)
{
  struct EquiniosLogger logger = g_instance;
  logger.ring_buffer_ = RingBuffer.new();
  logger.initialized_ = true;
  logger.log_level_ = LOG_LEVEL_INFO;
  return logger;
}

const struct EquiniosLoggerClass EquiniosLogger = {
    .instance = instanceEquiniosLogger,
    .new = newEquiniosLogger,
};
