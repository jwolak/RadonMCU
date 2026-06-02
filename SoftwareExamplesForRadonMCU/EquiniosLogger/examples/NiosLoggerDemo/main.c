#include "equinios.h"

#include <stdint.h>

static uint32_t g_demo_ticks = 0u;

static uint32_t demo_timestamp_provider(void)
{
  return g_demo_ticks;
}

int main(void)
{
  uint32_t loop = 0u;
  uint32_t dropped;

  log_set_level(LOG_LEVEL_TRACE);
  log_set_process_every_n_calls(1u);
  log_set_timestamp_provider(demo_timestamp_provider);

  LOGI("EquiniosLogger demo started");
  LOGI("Logger flush is called from main loop");

  while (1)
  {
    g_demo_ticks++;

    if ((loop % 1000u) == 0u)
    {
      LOGD("Heartbeat loop=%lu", (unsigned long)loop);
    }

    if ((loop % 5000u) == 0u)
    {
      LOGW("Warning sample loop=%lu", (unsigned long)loop);
    }

    if ((loop % 20000u) == 0u)
    {
      LOGE("Error sample loop=%lu", (unsigned long)loop);
    }

    if ((loop % 10000u) == 0u)
    {
      dropped = log_get_dropped_lines();
      if (dropped > 0u)
      {
        LOGW("Dropped log lines since last check: %lu", (unsigned long)dropped);
        log_reset_dropped_lines();
      }
    }

    log_process();
    loop++;
  }
}
