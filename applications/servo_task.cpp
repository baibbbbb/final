#include "cmsis_os.h"
#include "io/servo/servo.hpp"

// C板
sp::Servo servo(&htim1, TIM_CHANNEL_1, 168e6f, 180.0f);

extern "C" void servo_task()
{
    servo.start();

    servo.set(0.0f);

    float angle = 0.0f;
    const float step = 1.0f;         // 每步增加1度
    const int delay_ms = 1000 / 45;  // 每步时长，1秒转45度
    const int servo_start = 1;
    
    while (true) {       
        
        if (servo_start) {
            servo.set(angle);
            angle += step;
            if (angle > 180.0f) angle = 180.0f; // 限制最大角度
            osDelay(delay_ms);
        }
        
        osDelay(10);
    }
}