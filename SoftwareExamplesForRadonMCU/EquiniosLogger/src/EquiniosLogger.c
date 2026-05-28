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
#include "EquiniosLock.h"
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
#define EQUINIOS_LOG_LINE_EXTRA_CHARS 4u
#define EQUINIOS_LOG_PROCESS_EVERY_N_CALLS_DEFAULT 2u

static void ring_buffer_discard_oldest_line(struct EquiniosLogger *this)
{
  uint8_t byte;

  while (this->ring_buffer_.pop(&this->ring_buffer_, &byte))
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

    ring_buffer_discard_oldest_line(this);
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
  equinios_lock_state_t lock_state = EquiniosLock.enter();

  if (!this->initialized_)
  {
    this->ring_buffer_ = RingBuffer.new();
    this->timestamp_provider_ = TimestampProvider.new();
    this->initialized_ = true;
  }

  EquiniosLock.exit(lock_state);
}

/* public methods */
static void set_log_level(struct EquiniosLogger *this, log_level_t level)
{
  equinios_lock_state_t lock_state = EquiniosLock.enter();
  this->log_level_ = level;
  EquiniosLock.exit(lock_state);
}

static void set_process_every_n_calls(struct EquiniosLogger *this, uint32_t calls)
{
  equinios_lock_state_t lock_state;

  if (calls == 0u)
  {
    calls = 1u;
  }

  lock_state = EquiniosLock.enter();
  this->log_process_every_n_calls_ = calls;
  this->log_process_divider_ = 0u;
  EquiniosLock.exit(lock_state);
}

static void set_timestamp_provider(struct EquiniosLogger *this, uint32_t (*provider)(void))
{
  equinios_lock_state_t lock_state;

  logger_ensure_initialized(this);
  lock_state = EquiniosLock.enter();
  this->timestamp_provider_.set_provider(&this->timestamp_provider_, provider);
  EquiniosLock.exit(lock_state);
}

static void log_vwrite(struct EquiniosLogger *this, log_level_t level, const char *fmt,
                       va_list args)
{
  char line[EQUINIOS_LOG_MSG_MAX_LEN + EQUINIOS_LOG_LINE_EXTRA_CHARS];
  char message[EQUINIOS_LOG_MSG_MAX_LEN];
  uint32_t timestamp;
  equinios_lock_state_t lock_state;

  if (level > this->log_level_)
  {
    return;
  }

  logger_ensure_initialized(this);

  (void)vsnprintf(message, sizeof(message), fmt, args);

  lock_state = EquiniosLock.enter();
  timestamp = this->timestamp_provider_.get_timestamp(&this->timestamp_provider_);
  (void)snprintf(line, sizeof(line), "[%lu] %s\r\n", (unsigned long)timestamp, message);
  logger_enqueue_line(this, line);
  EquiniosLock.exit(lock_state);
}

static void log_write(struct EquiniosLogger *this, log_level_t level, const char *fmt, ...)
{
  va_list args;

  va_start(args, fmt);
  this->log_vwrite(this, level, fmt, args);
  va_end(args);
}

static void increment_log_process_divider(struct EquiniosLogger *this)
{
  equinios_lock_state_t lock_state = EquiniosLock.enter();
  this->log_process_divider_++;
  EquiniosLock.exit(lock_state);
}

static void reset_log_process_divider(struct EquiniosLogger *this)
{
  equinios_lock_state_t lock_state = EquiniosLock.enter();
  this->log_process_divider_ = 0u;
  EquiniosLock.exit(lock_state);
}

static struct EquiniosLogger g_instance = {
    .set_log_level = set_log_level,
    .set_process_every_n_calls = set_process_every_n_calls,
    .set_timestamp_provider = set_timestamp_provider,
    .log_vwrite = log_vwrite,
    .log_write = log_write,
    .initialized_ = false,
    .log_level_ = LOG_LEVEL_INFO,
    .log_process_divider_ = 0u,
    .log_process_every_n_calls_ = EQUINIOS_LOG_PROCESS_EVERY_N_CALLS_DEFAULT,
    .increment_log_process_divider = increment_log_process_divider,
    .reset_log_process_divider = reset_log_process_divider,
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
  logger.timestamp_provider_ = TimestampProvider.new();
  logger.initialized_ = true;
  logger.log_level_ = LOG_LEVEL_INFO;
  logger.log_process_divider_ = 0u;
  logger.log_process_every_n_calls_ = EQUINIOS_LOG_PROCESS_EVERY_N_CALLS_DEFAULT;
  logger.increment_log_process_divider = increment_log_process_divider;
  logger.reset_log_process_divider = reset_log_process_divider;
  return logger;
}

const struct EquiniosLoggerClass EquiniosLogger = {
    .instance = instanceEquiniosLogger,
    .new = newEquiniosLogger,
};
