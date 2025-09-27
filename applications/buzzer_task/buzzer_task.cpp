#include "cmsis_os.h"
#include "io/buzzer/buzzer.hpp"

// C板
sp::Buzzer buzzer(&htim4, TIM_CHANNEL_3, 84e6);

extern "C" void buzzer_task()
{
  //启动蜂鸣器
  buzzer.set(5000, 0.1);
  
  //烧录完成蜂鸣器响一声
  buzzer.start();
  osDelay(100);
  buzzer.stop();
  osDelay(100);

  //第一首歌曲
  
  

  while (true) {
    osDelay(100);
  }
}