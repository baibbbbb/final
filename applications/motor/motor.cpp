#include "motor.hpp"
#include "io/dbus/dbus.hpp"

extern sp::DBus remote;

float vx = remote.ch_lv * 3.0f; //底盘x方向速度, 单位: m/s
float vy = remote.ch_lh * 3.0f; //底盘y方向速度, 单位: m/s
float w = remote.ch_rh * 2.0f + remote.ch_rv * 2.0f;  //底盘角速度，单位: rad/s