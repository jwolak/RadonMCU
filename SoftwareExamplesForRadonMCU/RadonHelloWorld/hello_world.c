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
#include "altera_avalon_pio_regs.h"

#include <stdint.h>

int main()
{
  alt_putstr("Hello from Radon MCU!\n");

  uint32_t led_value = 0x1;
  int8_t direction = 1;

  while (1)
  {
    IOWR_ALTERA_AVALON_PIO_DATA(PIO_BASE, (~led_value) & 0xF);
    alt_busy_sleep(200000);

    if (direction > 0)
    {
      if (led_value == 0x8)
      {
        direction = -1;
        led_value >>= 1;
      }
      else
      {
        led_value <<= 1;
      }
    }
    else
    {
      if (led_value == 0x1)
      {
        direction = 1;
        led_value <<= 1;
      }
      else
      {
        led_value >>= 1;
      }
    }
  }

  return 0;
}
