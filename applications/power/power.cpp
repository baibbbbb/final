#include "power.hpp"
#include <cmath> // sqrt

namespace sp {
    
Power::Power(float k1, float k2, float k3, float pmax)
: k1_(k1), k2_(k2), k3_(k3), pmax_(pmax)
{
}

void Power::setPmax(float value) { pmax_ = value; }

void Power::calcpower(float set0, float pid_out0, 
                       float set1, float pid_out1, 
                       float set2, float pid_out2, 
                       float set3, float pid_out3)
{
    // 计算平方和与乘积和
    this->data.sum_set2 = set0 * set0 + set1 * set1 + set2 * set2 + set3 * set3;
    this->data.sum_pid_out2 = pid_out0 * pid_out0 + pid_out1 * pid_out1 + pid_out2 * pid_out2 + pid_out3 * pid_out3;
    this->data.sum_set_pid_out = set0 * pid_out0 + set1 * pid_out1 + set2 * pid_out2 + set3 * pid_out3;

    // 计算总功率
    this->data.p_in = this->data.sum_set_pid_out        // 机械功率
                    + k1_ * this->data.sum_pid_out2    // 铜损
                    + k2_ * this->data.sum_set2       // 铁损
                    + k3_;                             // 静态功耗

    // 判断是否超功率
    if (this->data.p_in < pmax_) {
        this->out = 1.0f; // 不缩放
    } else {
        // 计算判别式
        float discriminant = this->data.sum_set_pid_out * this->data.sum_set_pid_out
                            - 4.0f * k1_ * this->data.sum_pid_out2 * (k2_ * this->data.sum_set2 + k3_ - pmax_);
        if (discriminant < 0.0f) discriminant = 0.0f; // 防止负开方

        // 避免除以零
        float denom = 2.0f * k1_ * this->data.sum_pid_out2;
        if (denom == 0.0f) denom = 1e-6f;

        // 计算缩放系数
        this->data.k = (-this->data.sum_set_pid_out + sqrt(discriminant)) / denom;
        this->out = this->data.k;
    }
}
} // namespace sp

    