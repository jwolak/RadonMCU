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
#include "system.h"
#include "altera_avalon_timer_regs.h"
#include <stdio.h>

/* LED delay in microseconds (1s) */
#define LED_DELAY 1000000U /* 1s delay between LEDs */

/* Get elapsed microseconds within current 1ms timer period */
static uint32_t timer_get_period_us(void)
{
  /* Trigger snapshot by writing to SNAPH */
  IOWR_ALTERA_AVALON_TIMER_SNAPH(TIMER_0_BASE, 0);

  /* Read snapshot - shows remaining time to next timeout */
  uint32_t snap_l = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE);
  uint32_t snap_h = IORD_ALTERA_AVALON_TIMER_SNAPH(TIMER_0_BASE);

  uint32_t remaining = (snap_h << 16) | snap_l;

  /* Timer counts DOWN from 49999, so elapsed = 50000 - remaining */
  uint32_t elapsed_ticks = 50000 - remaining;

  /* Convert to microseconds: 50 ticks = 1us */
  return elapsed_ticks / 50;
}

/* Wait for specified microseconds using TIMER_0 polling */
static uint32_t timer_wait_us(uint32_t microseconds)
{
  uint32_t target_ms = (microseconds + 999) / 1000;
  uint32_t ms_count = 0;
  uint32_t prev_period_us = 0;
  int wraparound_count = 0;

  while (ms_count < target_ms && wraparound_count < target_ms * 5)
  {
    uint32_t curr_period_us = timer_get_period_us();

    if (prev_period_us > 500 && curr_period_us < 200)
    {
      ms_count++;
    }

    prev_period_us = curr_period_us;
    wraparound_count++;
  }

  return target_ms * 1000;
}

void run_knight_rider_cycle(struct KnightRiderLight *this)
{
  this->led_driver.set_led0_state(&this->led_driver, LED_ON);
  timer_wait_us(LED_DELAY);
  this->led_driver.set_led0_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led1_state(&this->led_driver, LED_ON);
  timer_wait_us(LED_DELAY);
  this->led_driver.set_led1_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led2_state(&this->led_driver, LED_ON);
  timer_wait_us(LED_DELAY);
  this->led_driver.set_led2_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led3_state(&this->led_driver, LED_ON);
  timer_wait_us(LED_DELAY);
  this->led_driver.set_led3_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led2_state(&this->led_driver, LED_ON);
  timer_wait_us(LED_DELAY);
  this->led_driver.set_led2_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led1_state(&this->led_driver, LED_ON);
  timer_wait_us(LED_DELAY);
  /* Keep last LED on so there is no dark pause between cycles. */
}

void run_knight_rider_cycle_smooth(struct KnightRiderLight *this)
{
  this->led_driver.set_led0_state(&this->led_driver, LED_ON);
  timer_wait_us(LED_DELAY);

  this->led_driver.set_led1_state(&this->led_driver, LED_ON);
  timer_wait_us(LED_DELAY);

  this->led_driver.set_led2_state(&this->led_driver, LED_ON);
  timer_wait_us(LED_DELAY);

  this->led_driver.set_led3_state(&this->led_driver, LED_ON);
  timer_wait_us(LED_DELAY);

  this->led_driver.set_led2_state(&this->led_driver, LED_ON);
  timer_wait_us(LED_DELAY);

  this->led_driver.set_led1_state(&this->led_driver, LED_ON);
  timer_wait_us(LED_DELAY);

  this->led_driver.set_led0_state(&this->led_driver, LED_ON);
  timer_wait_us(LED_DELAY);
}

static struct KnightRiderLight newKnightRiderLight(void)
{
  LOG_INFO("KnightRiderLight initialized");

  /* Start TIMER_0 with ITO, CONT and START bits */
  uint32_t control = ALTERA_AVALON_TIMER_CONTROL_ITO_MSK | ALTERA_AVALON_TIMER_CONTROL_CONT_MSK |
                     ALTERA_AVALON_TIMER_CONTROL_START_MSK;
  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, control);

  return (struct KnightRiderLight){
      .run_knight_rider_cycle = run_knight_rider_cycle,
      .run_knight_rider_cycle_smooth = run_knight_rider_cycle_smooth,
      .led_driver = LedDriver.new(),
  };
}

const struct KnightRiderLightClass KnightRiderLight = {
    .new = newKnightRiderLight,
};
