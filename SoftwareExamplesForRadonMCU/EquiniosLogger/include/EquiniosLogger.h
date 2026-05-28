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

#ifndef __EQUINIOSLOGGER_H_
#define __EQUINIOSLOGGER_H_

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include "RingBuffer.h"
#include "TimestampProvider.h"

#include "EquiniosTypes.h"

struct EquiniosLogger
{
  /* public members */
  void (*set_log_level)(struct EquiniosLogger *this, log_level_t level);
  void (*set_process_every_n_calls)(struct EquiniosLogger *this, uint32_t calls);
  void (*set_timestamp_provider)(struct EquiniosLogger *this, uint32_t (*provider)(void));
  void (*log_vwrite)(struct EquiniosLogger *this, log_level_t level, const char *fmt, va_list args);
  void (*increment_log_process_divider)(struct EquiniosLogger *this);
  void (*reset_log_process_divider)(struct EquiniosLogger *this);

  /* private members */
  bool initialized_;
  log_level_t log_level_;
  uint32_t log_process_divider_;
  uint32_t log_process_every_n_calls_;
  struct RingBuffer ring_buffer_;
  struct TimestampProvider timestamp_provider_;
};

extern const struct EquiniosLoggerClass
{
  /* Returns a pointer to a single global logger instance. */
  struct EquiniosLogger *(*instance)(void);

  struct EquiniosLogger (*new)();
} EquiniosLogger;

#endif /* __EQUINIOSLOGGER_H_ */
