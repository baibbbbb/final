#include "power.hpp"

namespace sp 
{
Power::Power(
float k1, float k2, float k3, float pmax)
: k1_(k1), k2_(k2), k3_(k3), pmax_(pmax)
{
}

void Power::calcpower(float set0, float pid_out0, 
                 float set1, float pid_out1, 
                 float set2, float pid_out2, 
                 float set3, float pid_out3)
{
    this->data.sum_set2 = (set0 * set0 + set1 * set1 + set2 * set2 + set3 * set3); //初始输出功率平方和

    this->data.sum_pid_out2 = (pid_out0 * pid_out0 + pid_out1 * pid_out1 + pid_out2 * pid_out2 + pid_out3 * pid_out3); //PID输出功率平方和

    this->data.sum_set_pid_out = set0 * pid_out0 + set1 *  pid_out1 + set2 * pid_out2 + set3 * pid_out3; //期望转速和PID输出功率和
    
    this->data.p_in = this->data.sum_set_pid_out //机械功率
                + k1_ * this->data.sum_pid_out2 //铜损
                + k2_ * this->data.sum_set2 //铁损
                + k3_; //静态待机消耗
    
    this->data.k = (- this->data.sum_set_pid_out + sqrt(this->data.sum_set_pid_out - 4 * k1_ * this->data.sum_pid_out2 * (k2_ * this->data.sum_set2 + k3_ - pmax_))) 
                / (2 * k1_ * this->data.sum_pid_out2); //转矩缩小系数
    
    if (this->data.p_in < pmax_) {
        this->out = 1.0f;
    } else {
        this->out = this->data.k;
    }

}  
}// namespace sp
    