#include "cmsis_os.h"
#include "io/can/can.hpp"
#include "io/plotter/plotter.hpp"
#include "motor/motor.hpp"
#include "motor/rm_motor/rm_motor.hpp"
#include "power/power.hpp"
#include "tools/pid/pid.hpp"
#include "motor/super_cap/super_cap.hpp"

sp::Plotter plotter(&huart1);

extern sp::PID rm_motor0_speed;
extern sp::PID rm_motor1_speed;
extern sp::PID rm_motor2_speed;
extern sp::PID rm_motor3_speed;

extern MotorData rm_motor0_data;
extern MotorData rm_motor1_data;
extern MotorData rm_motor2_data;
extern MotorData rm_motor3_data;

extern mx::Power power;
extern sp::SuperCap super_cap;

extern "C" void plotter_task()
{
  while (true) {
    plotter.plot(
      
      super_cap.power_in - super_cap.power_out,

      power.data.p_in 

    );

    osDelay(10);  // 100Hz
  }
}