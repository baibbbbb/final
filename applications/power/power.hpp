#ifndef SP__POWER_HPP
#define SP__POWER_HPP

#include "tools/math_tools/math_tools.hpp"

namespace sp 
{
struct PowerData
{
  float sum_set2; // 期望转速平方和
  float sum_pid_out2; // PID输出值_扭矩平方和
  float sum_set_pid_out; // 期望转速*PID输出值_扭矩和
  float k1; // 铜损常数
  float k2; // 铁损常数
  float k3; // 静态待机消耗
  float pmax; //最大功率
  float p_in; //输入轴功
  float k; //转矩缩小系数

};

class Power
{
public:
  
  Power(float k1, float k2, float k3, float pmax);

  float out = 0.0f; // 输出功率
  PowerData data; // 只读! 功率计算数据

  // 计算功率
  // pid_out: PID输出值_转矩
  // set: 目标值_转速
  void calcpower(float set0, float pid_out0, 
            float set1, float pid_out1, 
            float set2, float pid_out2, 
            float set3, float pid_out3);

private:
    const float k1_, k2_, k3_;
    const float pmax_;
    };
    
}  // namespace sp

#endif // SP__POWER_HPP

