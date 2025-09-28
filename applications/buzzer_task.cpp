#include "cmsis_os.h"
#include "io/buzzer/buzzer.hpp"

// C板
sp::Buzzer buzzer(&htim4, TIM_CHANNEL_3, 84e6);

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
    const Bate* song; 
    size_t song_length; //歌曲长度
    uint8_t play_state; //是否播放
    uint16_t bpm; //播放速度
    float note_duration; //每节音符时长
    uint16_t play_index = 0; //播放进度

    SongPlayer(const Bate* song, size_t song_length, uint8_t play_state, uint16_t bpm)
        : song(song), song_length(song_length), play_state(play_state), bpm(bpm)
    {
        note_duration = 1000.0f * 60.0f / this->bpm;
    }

   //播放函数
    void play() {
        if (play_state == 1) {
            if (play_index < song_length) {
                buzzer.set(song[play_index].frequency, 0.5f);
                buzzer.start();
                osDelay(song[play_index].period * note_duration);
                buzzer.stop();
                osDelay(50);
                play_index++;
            } 
            
            else {
                play_state = 0;
                play_index = 0;
            }
        }
    }
};

//歌曲Moon
const Bate Moon[] = {
    {L5, 0.5f}, {M1, 1.5f}, {M3, 0.5f}, {M5, 1.5f}, {M1,0.5f}, {L7, 1.5f}, {M3, 0.5f}, {M5, 1.0f}, {P0, 0.5f},
    {M5, 0.5f}, {M6, 1.5f}, {M7, 0.5f}, {H1, 1.5f}, {M6, 1.0f}, {M5, 1.5f}
};

// 使用示例：
// SongPlayer player(Moon, sizeof(Moon)/sizeof(Moon[0]), 1, 120);

extern "C" void buzzer_task()
{
    buzzer.set(5000, 0.1);
    
    for (int i = 0; i < 3; i++) {
     buzzer.start();
     osDelay(100);
     buzzer.stop();
     osDelay(100);
    }

    SongPlayer SongMoon(Moon, sizeof(Moon)/sizeof(Moon[0]), 1, 78);

    while (true) {
        //SongMoon.play();
        osDelay(100);
    }
}