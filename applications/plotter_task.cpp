#include "cmsis_os.h"
#include "io/can/can.hpp"
#include "io/plotter/plotter.hpp"
#include "motor/motor.hpp"
#include "motor/rm_motor/rm_motor.hpp"
#include "tools/pid/pid.hpp"

sp::Plotter plotter(&huart1);

extern sp::PID rm_motor0_speed;
extern sp::PID rm_motor1_speed;
extern sp::PID rm_motor2_speed;
extern sp::PID rm_motor3_speed;

extern MotorData rm_motor0_data;
extern MotorData rm_motor1_data;
extern MotorData rm_motor2_data;
extern MotorData rm_motor3_data;

extern "C" void plotter_task()
{
  while (true) {
    plotter.plot(

      motor0.speed, rm_motor0_data.absolute_speed_set,

      motor1.speed, rm_motor1_data.absolute_speed_set,

      motor2.speed, rm_motor2_data.absolute_speed_set,

      motor3.speed, rm_motor3_data.absolute_speed_set

    );

    osDelay(10);  // 100Hz
  }
}