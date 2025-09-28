#include "cmsis_os.h"
#include "io/led/led.hpp"

sp::LED led(&htim5);

extern "C" void led_task()
{
  led.start();

  while (true) {
    //彩色呼吸灯
    for (uint8_t g = 0; g < 10; g++) {
      led.set(0.1 - g * 0.01f, g * 0.01f, 0);
      osDelay(300);
    }

    for (uint8_t g = 0; g < 10 ; g++) {
      led.set(0, 0.1 - g * 0.01f, g * 0.01f);
      osDelay(300);
    }

    for (uint8_t g = 0; g < 10 ; g++) {
      led.set(g * 0.01f, 0, 0.1 - g * 0.01f);
      osDelay(300);
    }
  }
}