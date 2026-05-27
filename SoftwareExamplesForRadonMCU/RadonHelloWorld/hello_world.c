/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include "system.h"
#include "sys/alt_stdio.h"
#include "sys/alt_sys_wrappers.h"
#include "sys/alt_alarm.h"
#include "priv/alt_busy_sleep.h"
#include "altera_avalon_pio_regs.h"
#include <unistd.h>

#include "KnightRiderLight/KnightRiderLight.h"
#include "equinios.hpp"

#include <stdint.h>
#include <stdbool.h>

#define LED_DELAY 100000

static uint32_t hello_world_log_timestamp(void)
{
  return alt_nticks();
}

int main()
{
  alt_putstr("Hello from Radon MCU!\n");
  log_set_level(LOG_LEVEL_DEBUG);
  log_set_timestamp_provider(hello_world_log_timestamp);
  LOGI("EquiniosLogger singleton ready");

  struct KnightRiderLight knight_rider_light = KnightRiderLight.new();

  while (true)
  {
    log_process();

    uint32_t led_value = knight_rider_light.get_led_value(&knight_rider_light);
    IOWR_ALTERA_AVALON_PIO_DATA(PIO_BASE, (~led_value) & 0xF);
    alt_busy_sleep(LED_DELAY);
  }

  return 0;
}
