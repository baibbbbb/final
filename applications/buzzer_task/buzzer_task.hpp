#ifndef BUZZER_TASK_HPP
#define BUZZER_TASK_HPP

#include <cstdint>
#include "sp_middleware/io/buzzer/buzzer.hpp"

//定义音调频率
constexpr  int P0 = 0;	// 休止符频率

constexpr  int L1 = 262;	// 低音频率
constexpr  int L2 = 294;
constexpr  int L3 = 330;
constexpr  int L4 = 349;
constexpr  int L5 = 392;
constexpr  int L6 = 440;
constexpr  int L7 = 494;

constexpr  int M1 = 523;	// 中音频率
constexpr  int M2 = 587;
constexpr  int M3 = 659;
constexpr  int M4 = 698;	    
constexpr  int M5 = 784;
constexpr  int M6 = 880;
constexpr  int M7 = 988;

constexpr  int H1 = 1047;	// 高音频率
constexpr  int H2 = 1175;
constexpr  int H3 = 1319;
constexpr  int H4 = 1397;
constexpr  int H5 = 1568;
constexpr  int H6 = 1760;
constexpr  int H7 = 1976;

typedef struct{
      uint16_t frequency; // 音符频率
      float period; // 音符持续时间，单位为拍
  } Bate;

//播放状态
uint8_t playState = 0;

//播放进度
uint16_t playIndex = 0;

//节拍速度（每分钟多少拍）
uint8_t bpm = 132;

//每拍的持续时间
float noteDuration = 1000 * 60 / bpm;

//定义音符数组
const Bate Moon[] = {
  // 你问我爱你有多深
  {L5, 0.5f}, {M1, 1.5f}, {M3, 0.5f}, {M5, 1.5f}, {M1,0.5f}, {L7, 1.5f}, {M3, 0.5f}, {M5, 1.0f}, {P0, 0.5f},

  // 我爱你有几分
  {M5, 0.5f}, {M6, 1.5f}, {M7, 0.5f}, {H1, 1.5f}, {M6, 1.0f}, {M5, 1.5f}
};

#endif