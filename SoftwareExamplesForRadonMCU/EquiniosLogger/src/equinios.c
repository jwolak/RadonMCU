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

#include "equinios.h"
#include "EquiniosLogger.h"

#include <stdarg.h>
#include <stdio.h>

#define LOG_PROCESS_EVERY_N_CALLS 8u

static uint32_t g_log_process_divider = 0u;

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

void log_process(void)
{
  struct EquiniosLogger *logger = EquiniosLogger.instance();
  uint8_t byte;

  g_log_process_divider++;
  if (g_log_process_divider < LOG_PROCESS_EVERY_N_CALLS)
  {
    return;
  }

  g_log_process_divider = 0u;

  while (logger->ring_buffer_.pop(&logger->ring_buffer_, &byte))
  {
    putchar((int)byte);
  }
}
