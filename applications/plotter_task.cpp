#include "cmsis_os.h"
#include "io/can/can.hpp"
#include "io/plotter/plotter.hpp"
#include "motor/rm_motor/rm_motor.hpp"
#include "motor/motor.hpp"

sp::Plotter plotter(&huart1);

extern "C" void plotter_task()
{
  while (true) {
    plotter.plot(

      motor0.speed, 
      motor0.torque,

      motor1.speed, 
      motor1.torque,

      motor2.speed, 
      motor2.torque,

      motor3.speed,
      motor3.torque

    );

    osDelay(10);  // 100Hz
  }
}