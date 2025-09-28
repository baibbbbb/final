#include "cmsis_os.h"
#include "io/buzzer/buzzer.hpp"
#include "buzzer_task.hpp"

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
  //定义音符数组
  const Bate Moon[] = {
  // 你问我爱你有多深
  {L5, 0.5f}, {M1, 1.5f}, {M3, 0.5f}, {M5, 1.5f}, {M1,0.5f}, {L7, 1.5f}, {M3, 0.5f}, {M5, 1.0f}, {P0, 0.5f},

  // 我爱你有几分
  {M5, 0.5f}, {M6, 1.5f}, {M7, 0.5f}, {H1, 1.5f}, {M6, 1.0f}, {M5, 1.5f}

  };

  SongPlayer Moon(Moon, 1, 78);

  Moon.play();

  while (true) {
    
    osDelay(100);
  
  }

}