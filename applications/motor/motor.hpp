#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "io/dbus/dbus.hpp"
#include "motor/rm_motor/rm_motor.hpp"

extern sp::DBus remote;

constexpr float a = 0.185f;  //电机x坐标
constexpr float b = 0.165f;  //电机y坐标
constexpr float s = 0.154f;  //麦轮半径

extern float vx; //底盘x方向速度
extern float vy; //底盘y方向速度
extern float w; //底盘角速度

//从底盘左上逆时针一周分别为motor0,1,2,3
inline sp::RM_Motor motor3(1, sp::RM_Motors::M3508, 14.9f, true);
inline sp::RM_Motor motor0(2, sp::RM_Motors::M3508, 14.9f, true);
inline sp::RM_Motor motor1(3, sp::RM_Motors::M3508, 14.9f, true);
inline sp::RM_Motor motor2(4, sp::RM_Motors::M3508, 14.9f, true);

//麦轮底盘运动逆解算
typedef struct
{
  float w0;  //电机0速度，单位: rad/s
  float w1;  //电机1速度，单位: rad/s
  float w2;  //电机2速度，单位: rad/s
  float w3;  //电机3速度，单位: rad/s
} MotorSpeed;

typedef struct
{
  float absolute_speed_set;  // 绝对速度的目标值，rad/s
  float given_torque;        // 电机给定的力矩，Nm
} MotorData;

#endif  // MOTOR_HPP