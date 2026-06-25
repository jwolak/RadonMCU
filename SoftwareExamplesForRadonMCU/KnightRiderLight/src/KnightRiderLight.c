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

/* Get current timer value in microseconds */
static uint32_t timer_get_us(void)
{
  /* Trigger snapshot by writing to SNAPH */
  IOWR_ALTERA_AVALON_TIMER_SNAPH(TIMER_0_BASE, 0);

  /* Read snapshot registers */
  uint32_t snap_h = IORD_ALTERA_AVALON_TIMER_SNAPH(TIMER_0_BASE);
  uint32_t snap_l = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE);

  uint32_t timerValue = (snap_h << 16) | snap_l;

  /* Timer period is 49999 ticks (1ms at 50MHz) */
  /* Timer counts down from 49999 to 0 */
  /* Elapsed microseconds in current period = (50000 - timerValue) / 50 */
  uint32_t elapsed_us = (50000 - (timerValue % 50000)) / 50;

  return elapsed_us;
}

/* Wait for specified microseconds using TIMER_0 */
static uint32_t timer_wait_us(uint32_t microseconds)
{
  uint32_t start = timer_get_us();
  uint32_t current;
  uint32_t elapsed;
  uint32_t loop_count = 0;

  printf("Timer wait start: %lu us for %lu us\r\n", start, microseconds);

  do
  {
    current = timer_get_us();
    loop_count++;

    /* Handle potential timer overflow */
    if (current >= start)
    {
      elapsed = current - start;
    }
    else
    {
      elapsed = (0xFFFFFFFFUL - start) + current;
    }

    /* Safety timeout - 100ms */
    if (loop_count > 100000000)
    {
      printf("TIMEOUT! loops=%lu, elapsed=%lu us\r\n", loop_count, elapsed);
      break;
    }
  } while (elapsed < microseconds);

  printf("Timer wait end: elapsed=%lu us, loops=%lu\r\n", elapsed, loop_count);
  return elapsed;
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
