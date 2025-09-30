#include "cmsis_os.h"
#include "io/can/can.hpp"
#include "io/dbus/dbus.hpp"
#include "motor/motor.hpp"
#include "motor/rm_motor/rm_motor.hpp"
#include "tools/pid/pid.hpp"

extern sp::DBus remote;

sp::CAN can1(&hcan1);

//                         dt     kp    ki    kd    mo   mio   alpha  ang? dynamic?
sp::PID rm_motor0_speed(0.001f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 1.0f, false, true);
sp::PID rm_motor1_speed(0.001f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 1.0f, false, true);
sp::PID rm_motor2_speed(0.001f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 1.0f, false, true);
sp::PID rm_motor3_speed(0.001f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 1.0f, false, true);

extern "C" void can_task()
{
  can1.config();
  can1.start();
  remote.request();

  MotorData rm_motor0_data;
  MotorData rm_motor1_data;  
  MotorData rm_motor2_data;  
  MotorData rm_motor3_data;    
  
  // 初始化
  rm_motor0_data.absolute_speed_set = 0.0f;
  rm_motor0_data.given_torque = 0.0f;

  rm_motor1_data.absolute_speed_set = 0.0f;
  rm_motor1_data.given_torque = 0.0f;

  rm_motor2_data.absolute_speed_set = 0.0f;
  rm_motor2_data.given_torque = 0.0f;

  rm_motor3_data.absolute_speed_set = 0.0f;
  rm_motor3_data.given_torque = 0.0f;

  MotorSpeed motor_speed;

  while (true) {

    rm_motor0_data.absolute_speed_set = motor_speed.w0;
    rm_motor1_data.absolute_speed_set = motor_speed.w1;
    rm_motor2_data.absolute_speed_set = motor_speed.w2;
    rm_motor3_data.absolute_speed_set = motor_speed.w3;

    // 计算PID输出值
    switch (remote.sw_r) {
      case sp::DBusSwitchMode::MID:
        rm_motor0_speed.calc(
          rm_motor0_data.absolute_speed_set, motor0.speed);  // 设定值，反馈值
        rm_motor0_data.given_torque = rm_motor0_speed.out;           // 扭矩

        rm_motor1_speed.calc(
          rm_motor1_data.absolute_speed_set, motor1.speed);  // 设定值，反馈值
        rm_motor1_data.given_torque = rm_motor1_speed.out;           // 扭矩

        rm_motor2_speed.calc(
          rm_motor2_data.absolute_speed_set, motor2.speed);  // 设定值，反馈值
        rm_motor2_data.given_torque = rm_motor2_speed.out;           // 扭矩

        rm_motor3_speed.calc(
          rm_motor3_data.absolute_speed_set, motor3.speed);  // 设定值，反馈值
        rm_motor3_data.given_torque = rm_motor3_speed.out;           // 扭矩
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

    motor0.cmd(rm_motor0_data.given_torque);
    motor0.write(can1.tx_data);
    can1.send(motor0.tx_id);

    motor1.cmd(rm_motor1_data.given_torque);
    motor1.write(can1.tx_data);
    can1.send(motor1.tx_id);

    motor2.cmd(rm_motor2_data.given_torque);
    motor2.write(can1.tx_data);
    can1.send(motor2.tx_id);

    motor3.cmd(rm_motor3_data.given_torque);
    motor3.write(can1.tx_data);
    can1.send(motor3.tx_id);

    osDelay(1);
  }
}

extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef * hcan)
{
  auto stamp_ms = osKernelSysTick();

  while (HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0) > 0) {
    if (hcan == &hcan1) {
      can1.recv();

      // 接收各个电机信息
      if (can1.rx_id == motor0.rx_id) motor0.read(can1.rx_data, stamp_ms);
      if (can1.rx_id == motor1.rx_id) motor1.read(can1.rx_data, stamp_ms);
      if (can1.rx_id == motor2.rx_id) motor2.read(can1.rx_data, stamp_ms);
      if (can1.rx_id == motor3.rx_id) motor3.read(can1.rx_data, stamp_ms);
    }
  }
}
