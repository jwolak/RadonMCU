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

/* LED delay in microseconds (2ms) */
#define LED_DELAY 2000U

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
  uint32_t start_period_us = timer_get_period_us();
  uint32_t end_period_us;
  uint32_t total_ms = 0;
  uint32_t target_ms = (microseconds + 999) / 1000; /* Round up to milliseconds */

  printf("Timer: wait for %lu us (~%lu ms)\r\n", microseconds, target_ms);

  /* Wait for target number of milliseconds */
  while (total_ms < target_ms)
  {
    end_period_us = timer_get_period_us();

    /* Detect ms boundary: when timer resets, elapsed goes from ~1000 back to 0 */
    /* Check for wraparound: if end < start AND start is high, ms boundary crossed */
    if (end_period_us < start_period_us && start_period_us > 500)
    {
      total_ms++;
      printf("  MS %lu passed (wrap: %lu -> %lu)\r\n", total_ms, start_period_us, end_period_us);
      start_period_us = end_period_us; /* Update reference */
    }

    /* Also track if we're advancing but haven't wrapped yet */
    if (end_period_us >= start_period_us)
    {
      start_period_us = end_period_us;
    }
  }

  printf("Timer: wait complete\r\n");
  return total_ms * 1000;
}

void run_knight_rider_cycle(struct KnightRiderLight *this)
{
  this->led_driver.set_led0_state(&this->led_driver, LED_ON);
  printf("LED0 delay: %lu us\r\n", timer_wait_us(LED_DELAY));
  this->led_driver.set_led0_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led1_state(&this->led_driver, LED_ON);
  printf("LED1 delay: %lu us\r\n", timer_wait_us(LED_DELAY));
  this->led_driver.set_led1_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led2_state(&this->led_driver, LED_ON);
  printf("LED2 delay: %lu us\r\n", timer_wait_us(LED_DELAY));
  this->led_driver.set_led2_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led3_state(&this->led_driver, LED_ON);
  printf("LED3 delay: %lu us\r\n", timer_wait_us(LED_DELAY));
  this->led_driver.set_led3_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led2_state(&this->led_driver, LED_ON);
  printf("LED2 delay: %lu us\r\n", timer_wait_us(LED_DELAY));
  this->led_driver.set_led2_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led1_state(&this->led_driver, LED_ON);
  printf("LED1 delay: %lu us\r\n", timer_wait_us(LED_DELAY));
  /* Keep last LED on so there is no dark pause between cycles. */
}

void run_knight_rider_cycle_smooth(struct KnightRiderLight *this)
{
  this->led_driver.set_led0_state(&this->led_driver, LED_ON);
  printf("LED0 smooth delay: %lu us\r\n", timer_wait_us(LED_DELAY));

  this->led_driver.set_led1_state(&this->led_driver, LED_ON);
  printf("LED1 smooth delay: %lu us\r\n", timer_wait_us(LED_DELAY));

  this->led_driver.set_led2_state(&this->led_driver, LED_ON);
  printf("LED2 smooth delay: %lu us\r\n", timer_wait_us(LED_DELAY));

  this->led_driver.set_led3_state(&this->led_driver, LED_ON);
  printf("LED3 smooth delay: %lu us\r\n", timer_wait_us(LED_DELAY));

  this->led_driver.set_led2_state(&this->led_driver, LED_ON);
  printf("LED2 smooth delay: %lu us\r\n", timer_wait_us(LED_DELAY));

  this->led_driver.set_led1_state(&this->led_driver, LED_ON);
  printf("LED1 smooth delay: %lu us\r\n", timer_wait_us(LED_DELAY));

  this->led_driver.set_led0_state(&this->led_driver, LED_ON);
  printf("LED0 smooth delay: %lu us\r\n", timer_wait_us(LED_DELAY));
}

static struct KnightRiderLight newKnightRiderLight(void)
{
  LOG_INFO("KnightRiderLight initialized");

  /* Start TIMER_0 with ITO, CONT and START bits */
  uint32_t control = ALTERA_AVALON_TIMER_CONTROL_ITO_MSK | ALTERA_AVALON_TIMER_CONTROL_CONT_MSK |
                     ALTERA_AVALON_TIMER_CONTROL_START_MSK;
  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, control);
  printf("TIMER_0 started with ITO, CONT, START\r\n");

  return (struct KnightRiderLight){
      .run_knight_rider_cycle = run_knight_rider_cycle,
      .run_knight_rider_cycle_smooth = run_knight_rider_cycle_smooth,
      .led_driver = LedDriver.new(),
  };
}

const struct KnightRiderLightClass KnightRiderLight = {
    .new = newKnightRiderLight,
};
