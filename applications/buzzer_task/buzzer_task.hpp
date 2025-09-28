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

class SongPlayer {
public:
    const Bate* song;      // 歌曲数组指针
    size_t songLength;     // 歌曲长度
    uint8_t playState;     // 播放状态
    uint16_t bpm;          // 节拍速度，每分钟拍多少拍
    float noteDuration;    // 音符持续时间，单位为毫秒
    uint16_t playIndex = 0; // 播放进度

    SongPlayer(const Bate* song, uint8_t playState, uint16_t bpm)
        : song(song), playState(playState), bpm(bpm)
    {
        // 计算歌曲长度
        songLength = sizeof(Moon) / sizeof(Moon[0]);
        noteDuration = 1000.0f * 60.0f / this->bpm;
    }

    void play() {
        if (playState == 1) {
            if (playIndex < songLength) {
                buzzer.set(song[playIndex].frequency, 0.5f);
                buzzer.start();
                osDelay(song[playIndex].period * noteDuration); //音符间隔
                buzzer.stop();
                osDelay(50); 
                playIndex++;
            } else {
                playState = 0; // 播放结束，重置状态
                playIndex = 0; // 重置播放索引
            }
        }
    }
};

// 使用示例：
// SongPlayer player(Moon, 1, 120);

#endif // BUZZER_TASK_HPP