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

static log_level_t g_log_level = LOG_LEVEL_INFO;

static void set_log_level(struct EquiniosLogger *this EQUINIOS_UNUSED, log_level_t level)
{
  g_log_level = level;
}

static void log_vwrite(struct EquiniosLogger *this EQUINIOS_UNUSED, log_level_t level,
                       const char *fmt, va_list args)
{
  if (level > g_log_level)
  {
    return;
  }

  vprintf(fmt, args);
  printf("\r\n");
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
    .log_vwrite = log_vwrite,
    .log_write = log_write,
};

static struct EquiniosLogger *instanceEquiniosLogger(void)
{
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