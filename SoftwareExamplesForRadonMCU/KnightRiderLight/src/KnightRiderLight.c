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
#include "altera_avalon_timer_regs.h"
#include <stdio.h>

/* LED delay in microseconds (2ms) */
#define LED_DELAY 2000U

/* Get current timer value in microseconds */
static uint32_t timer_get_us(void)
{
  uint32_t snap_l = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE);
  uint32_t snap_h = IORD_ALTERA_AVALON_TIMER_SNAPH(TIMER_0_BASE);
  uint32_t counter = (snap_h << 16) | snap_l;
  /* TIMER_0 frequency is 50MHz, period = 20ns, so 50 ticks = 1us */
  return counter / 50;
}

/* Wait for specified microseconds using TIMER_0, returns actual elapsed time */
static uint32_t timer_wait_us(uint32_t microseconds)
{
  uint32_t start = timer_get_us();
  uint32_t elapsed;
  do
  {
    elapsed = timer_get_us() - start;
  } while (elapsed < microseconds);
  return elapsed;
}

void run_knight_rider_cycle(struct KnightRiderLight *this)
{
  this->led_driver.set_led0_state(&this->led_driver, LED_ON);
  printf("LED0 delay: %u us\r\n", timer_wait_us(LED_DELAY));
  this->led_driver.set_led0_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led1_state(&this->led_driver, LED_ON);
  printf("LED1 delay: %u us\r\n", timer_wait_us(LED_DELAY));
  this->led_driver.set_led1_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led2_state(&this->led_driver, LED_ON);
  printf("LED2 delay: %u us\r\n", timer_wait_us(LED_DELAY));
  this->led_driver.set_led2_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led3_state(&this->led_driver, LED_ON);
  printf("LED3 delay: %u us\r\n", timer_wait_us(LED_DELAY));
  this->led_driver.set_led3_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led2_state(&this->led_driver, LED_ON);
  printf("LED2 delay: %u us\r\n", timer_wait_us(LED_DELAY));
  this->led_driver.set_led2_state(&this->led_driver, LED_OFF);

  this->led_driver.set_led1_state(&this->led_driver, LED_ON);
  printf("LED1 delay: %u us\r\n", timer_wait_us(LED_DELAY));
  /* Keep last LED on so there is no dark pause between cycles. */
}

void run_knight_rider_cycle_smooth(struct KnightRiderLight *this)
{
  this->led_driver.set_led0_state(&this->led_driver, LED_ON);
  printf("LED0 smooth delay: %u us\r\n", timer_wait_us(LED_DELAY));

  this->led_driver.set_led1_state(&this->led_driver, LED_ON);
  printf("LED1 smooth delay: %u us\r\n", timer_wait_us(LED_DELAY));

  this->led_driver.set_led2_state(&this->led_driver, LED_ON);
  printf("LED2 smooth delay: %u us\r\n", timer_wait_us(LED_DELAY));

  this->led_driver.set_led3_state(&this->led_driver, LED_ON);
  printf("LED3 smooth delay: %u us\r\n", timer_wait_us(LED_DELAY));

  this->led_driver.set_led2_state(&this->led_driver, LED_ON);
  printf("LED2 smooth delay: %u us\r\n", timer_wait_us(LED_DELAY));

  this->led_driver.set_led1_state(&this->led_driver, LED_ON);
  printf("LED1 smooth delay: %u us\r\n", timer_wait_us(LED_DELAY));

  this->led_driver.set_led0_state(&this->led_driver, LED_ON);
  printf("LED0 smooth delay: %u us\r\n", timer_wait_us(LED_DELAY));
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