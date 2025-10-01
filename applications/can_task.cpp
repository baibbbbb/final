#include "cmsis_os.h"
#include "io/can/can.hpp"
#include "io/dbus/dbus.hpp"
#include "motor/motor.hpp"
#include "motor/rm_motor/rm_motor.hpp"
#include "motor/super_cap/super_cap.hpp"
#include "power/power.hpp"
#include "tools/pid/pid.hpp"
#include "referee/pm02/pm02.hpp"

extern sp::DBus remote;
extern sp::PM02 pm02;

sp::CAN can2(&hcan2);

//                         dt     kp    ki    kd    mo   mio   alpha  ang? dynamic?
sp::PID rm_motor0_speed(0.001f, 0.7f, 0.5f, 0.0f, 3.0f, 0.0f, 1.0f, false, true);
sp::PID rm_motor1_speed(0.001f, 0.7f, 0.5f, 0.0f, 3.0f, 0.0f, 1.0f, false, true);
sp::PID rm_motor2_speed(0.001f, 0.7f, 0.5f, 0.0f, 3.0f, 0.0f, 1.0f, false, true);
sp::PID rm_motor3_speed(0.001f, 0.7f, 0.5f, 0.0f, 3.0f, 0.0f, 1.0f, false, true);

float dynamic_pmax = pm02.robot_status.chassis_power_limit;  // 动态功率限制

sp::Power power(2.5f, 0.01f, 7.6f, 80);  //功率限制
sp::SuperCap super_cap(sp::SuperCapMode::AUTOMODE);

MotorData rm_motor0_data;
MotorData rm_motor1_data;
MotorData rm_motor2_data;
MotorData rm_motor3_data;

float vx = 0.0f;
float vy = 0.0f;
float w = 0.0f;

MotorSpeed motor_speed;

extern "C" void can_task()
{
  can2.config();
  can2.start();
  remote.request();

  // 初始化

  rm_motor0_data.absolute_speed_set = 0.0f;
  rm_motor0_data.given_torque = 0.0f;

  rm_motor1_data.absolute_speed_set = 0.0f;
  rm_motor1_data.given_torque = 0.0f;

  rm_motor2_data.absolute_speed_set = 0.0f;
  rm_motor2_data.given_torque = 0.0f;

  rm_motor3_data.absolute_speed_set = 0.0f;
  rm_motor3_data.given_torque = 0.0f;

  while (true) {
    vx = -remote.ch_lv * 5.0f;                      //底盘x方向速度, 单位: m/s
    vy = remote.ch_lh * 5.0f;                       //底盘y方向速度, 单位: m/s
    w = remote.ch_rh * 5.0f + remote.ch_rv * 5.0f;  //底盘角速度，单位: rad/s

    motor_speed.w0 = (-vx + vy + (a + b) * w) / s;  //电机0速度，单位: rad/s
    motor_speed.w1 = (-vx - vy + (a + b) * w) / s;  //电机1速度，单位: rad/s
    motor_speed.w2 = (vx - vy + (a + b) * w) / s;   //电机2速度，单位: rad/s
    motor_speed.w3 = (vx + vy + (a + b) * w) / s;   //电机3速度，单位: rad/s

    rm_motor0_data.absolute_speed_set = motor_speed.w0;
    rm_motor1_data.absolute_speed_set = motor_speed.w1;
    rm_motor2_data.absolute_speed_set = motor_speed.w2;
    rm_motor3_data.absolute_speed_set = motor_speed.w3;

    // 计算PID输出值

    switch (remote.sw_r) {
      case sp::DBusSwitchMode::MID:
        // 功率限制

        rm_motor0_speed.calc(rm_motor0_data.absolute_speed_set, motor0.speed);  // 设定值，反馈值

        rm_motor1_speed.calc(rm_motor1_data.absolute_speed_set, motor1.speed);  // 设定值，反馈值

        rm_motor2_speed.calc(rm_motor2_data.absolute_speed_set, motor2.speed);  // 设定值，反馈值

        rm_motor3_speed.calc(rm_motor3_data.absolute_speed_set, motor3.speed);  // 设定值，反馈值

        power.calcpower(
          motor0.speed, rm_motor0_speed.out,
          motor1.speed, rm_motor1_speed.out, 
          motor2.speed, rm_motor2_speed.out,
          motor3.speed, rm_motor3_speed.out);

        rm_motor0_data.given_torque = power.out * rm_motor0_speed.out;
        rm_motor1_data.given_torque = power.out * rm_motor1_speed.out;
        rm_motor2_data.given_torque = power.out * rm_motor2_speed.out;
        rm_motor3_data.given_torque = power.out * rm_motor3_speed.out;
        break;

      case sp::DBusSwitchMode::DOWN:
        rm_motor0_data.given_torque = 0.0f;
        rm_motor1_data.given_torque = 0.0f;
        rm_motor2_data.given_torque = 0.0f;
        rm_motor3_data.given_torque = 0.0f;
        break;

      default:
        break;
    }

  if (super_cap.is_alive(osKernelSysTick())) {
    switch (remote.sw_l) {
        case sp::DBusSwitchMode::UP:
           // 自动模式
            if (super_cap.cap_energy > 62.5f) {
                dynamic_pmax = 200.0f; // 电容有电，放宽功率限制
            } else if (super_cap.cap_energy < 62.5f) {
                dynamic_pmax = pm02.robot_status.chassis_power_limit;  // 电量低，收紧功率
            }
            break;

        case sp::DBusSwitchMode::MID:  // 仅充电
            dynamic_pmax = pm02.robot_status.chassis_power_limit;      // 固定上限
            break;
    }
}

    motor0.cmd(rm_motor0_data.given_torque);
    motor0.write(can2.tx_data);
    can2.send(motor0.tx_id);

    motor1.cmd(rm_motor1_data.given_torque);
    motor1.write(can2.tx_data);
    can2.send(motor1.tx_id);

    motor2.cmd(rm_motor2_data.given_torque);
    motor2.write(can2.tx_data);
    can2.send(motor2.tx_id);

    motor3.cmd(rm_motor3_data.given_torque);
    motor3.write(can2.tx_data);
    can2.send(motor3.tx_id);

    osDelay(1);
  }
}

extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef * hcan)
{
  auto stamp_ms = osKernelSysTick();

  while (HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0) > 0) {
    if (hcan == &hcan2) {
      can2.recv();

      // 接收各个电机信息
      if (can2.rx_id == motor0.rx_id) motor0.read(can2.rx_data, stamp_ms);
      if (can2.rx_id == motor1.rx_id) motor1.read(can2.rx_data, stamp_ms);
      if (can2.rx_id == motor2.rx_id) motor2.read(can2.rx_data, stamp_ms);
      if (can2.rx_id == motor3.rx_id) motor3.read(can2.rx_data, stamp_ms);

      // 接收超级电容信息
      if (can2.rx_id == super_cap.rx_id) super_cap.read(can2.rx_data, stamp_ms);
    }
  }
}
