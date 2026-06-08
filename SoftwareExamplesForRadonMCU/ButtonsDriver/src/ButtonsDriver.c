/*-
 * BSD 3-Clause License
 *
 * Copyrights 2026, Janusz Wolak
 * All rights reserved.
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

#include "ButtonsDriver.h"
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "equinios.hpp"

#define BUTTON_RIGHT_MASK (1u << 2)
#define BUTTON_LEFT_MASK (1u << 1)
#define BUTTON_RESET_MASK (1u << 0)

ButtonState get_reset_button_status(struct ButtonsDriver *self)
{
  LOG_TRACE("[ButtonsDriver] get_reset_button_status() called...");
  return self->button_state_reader.get_button_status(BUTTON_RESET_MASK);
}

ButtonState get_left_button_status(struct ButtonsDriver *self)
{
  LOG_TRACE("[ButtonsDriver] get_left_button_status() called...");
  return self->button_state_reader.get_button_status(BUTTON_LEFT_MASK);
}

ButtonState get_right_button_status(struct ButtonsDriver *self)
{
  LOG_TRACE("[ButtonsDriver] get_right_button_status() called...");
  return self->button_state_reader.get_button_status(BUTTON_RIGHT_MASK);
}

static struct ButtonsDriver newButtonsDriver(void)
{
  struct ButtonsDriver driver;

  driver.get_reset_button_status = get_reset_button_status;
  driver.get_left_button_status = get_left_button_status;
  driver.get_right_button_status = get_right_button_status;

  return driver;
}

const struct ButtonsDriverClass ButtonsDriver = {.new = newButtonsDriver};