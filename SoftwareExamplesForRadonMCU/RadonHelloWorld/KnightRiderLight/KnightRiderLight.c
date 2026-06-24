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

#include "KnightRiderLight.h"
#include "equinios.hpp"
#include "LedDriver.h"
#include "priv/alt_busy_sleep.h"

#define LED_DELAY 100000U

void run_knight_rider_cycle(struct KnightRiderLight *this)
{
  this->led_driver.set_led0_state(&this->led_driver, LED_ON);
  alt_busy_sleep(LED_DELAY);
  this->led_driver.set_led0_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led1_state(&this->led_driver, LED_ON);
  alt_busy_sleep(LED_DELAY);
  this->led_driver.set_led1_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led2_state(&this->led_driver, LED_ON);
  alt_busy_sleep(LED_DELAY);
  this->led_driver.set_led2_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led3_state(&this->led_driver, LED_ON);
  alt_busy_sleep(LED_DELAY);
  this->led_driver.set_led3_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led2_state(&this->led_driver, LED_ON);
  alt_busy_sleep(LED_DELAY);
  this->led_driver.set_led2_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led1_state(&this->led_driver, LED_ON);
  alt_busy_sleep(LED_DELAY);
  /* Keep last LED on so there is no dark pause between cycles. */
}

void run_knight_rider_cycle_smooth(struct KnightRiderLight *this)
{
  this->led_driver.set_led0_state(&this->led_driver, LED_ON);
  alt_busy_sleep(LED_DELAY);

  this->led_driver.set_led1_state(&this->led_driver, LED_ON);
  alt_busy_sleep(LED_DELAY);

  this->led_driver.set_led2_state(&this->led_driver, LED_ON);
  alt_busy_sleep(LED_DELAY);

  this->led_driver.set_led3_state(&this->led_driver, LED_ON);
  alt_busy_sleep(LED_DELAY);

  this->led_driver.set_led2_state(&this->led_driver, LED_ON);
  alt_busy_sleep(LED_DELAY);

  this->led_driver.set_led1_state(&this->led_driver, LED_ON);
  alt_busy_sleep(LED_DELAY);

  this->led_driver.set_led0_state(&this->led_driver, LED_ON);
  alt_busy_sleep(LED_DELAY);
}

static struct KnightRiderLight newKnightRiderLight(void)
{
  LOG_INFO("KnightRiderLight initialized");

  return (struct KnightRiderLight){
      .run_knight_rider_cycle = run_knight_rider_cycle,
      .run_knight_rider_cycle_smooth = run_knight_rider_cycle_smooth,
      .led_driver = LedDriver.new(),
  };
}

const struct KnightRiderLightClass KnightRiderLight = {
    .new = newKnightRiderLight,
};