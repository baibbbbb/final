#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "motor/rm_motor/rm_motor.hpp"
#include "io/dbus/dbus.hpp"

extern sp::DBus remote;

constexpr float P_MIN = -3.141593f * 4;
constexpr float P_MAX = 3.141593f * 4;
constexpr float V_MIN = -30.0f;
constexpr float V_MAX = 30.0f;
constexpr float T_MIN = -10.0f;
constexpr float T_MAX = 10.0f;

constexpr float a = 0.185f; //电机x坐标
constexpr float b = 0.165f; //电机y坐标
constexpr float s = 0.154f;  //麦轮半径

float vx = remote.ch_lv * 3.0f; //底盘x方向速度, 单位: m/s
float vy = remote.ch_lh * 3.0f; //底盘y方向速度, 单位: m/s
float w = remote.ch_rh * 2.0f + remote.ch_rv * 2.0f;  //底盘角速度，单位: rad/s

inline sp::RM_Motor motor0(1, sp::RM_Motors::M3508);
inline sp::RM_Motor motor1(2, sp::RM_Motors::M3508);
inline sp::RM_Motor motor2(3, sp::RM_Motors::M3508);
inline sp::RM_Motor motor3(4, sp::RM_Motors::M3508);

//麦轮底盘运动逆解算
typedef struct {
  
  float w0 = (-vx + vy + (a + b) * w) / s; //电机0速度，单位: rad/s
  float w1 = (-vx - vy + (a + b) * w) / s; //电机1速度，单位: rad/s
  float w2 = (vx - vy + (a + b) * w) / s; //电机2速度，单位: rad/s
  float w3 = (vx + vy + (a + b) * w) / s; //电机3速度，单位: rad/s

} MotorSpeed;

typedef struct{
	float absolute_speed_set;	// 绝对速度的目标值，rad/s
	float given_torque;			// 电机给定的力矩，Nm
} MotorData;

MotorData rm_motor0_data;
MotorData rm_motor1_data;  
MotorData rm_motor2_data;  
MotorData rm_motor3_data;    


#endif  // MOTOR_HPP