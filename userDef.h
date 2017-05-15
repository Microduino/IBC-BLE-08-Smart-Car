#define DEBUG        0       //串口调试功能  打开/关闭.
#define PIN_BUZZER   10      //蜂鸣器引脚.
#define PIN_LED      12      //彩灯引脚.
#define PIN_LINEA    A0      //灰度传感器A引脚.
#define PIN_LINEB    A2      //灰度传感器B引脚.
#define PIN_MIC      A6      //声音传感器引脚.
#define LED_NUM      2       //彩灯数量
#define DOCK_MAX     850     //灰度传感器检测的触发值
#define TRACK_THRESHOLD  500
#define Crash_THRESHOLD  850

#define BRIGHT_MAX  128      //LED彩灯显示的最大亮度。最大亮度是255，最小亮度是0

#define MAX_THROTTLE  150   //小车的移动速度. 设置范围-255~255，255对应最快的转速，负值意味着小车倒着走，-255对应反方向的最快速度，0对应停止
#define STEER  30
