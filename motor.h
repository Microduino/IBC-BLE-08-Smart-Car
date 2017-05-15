#include <Microduino_Motor.h>
#if defined(__AVR_ATmega32U4__) || (__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define motor_pin0A 8
#define motor_pin0B 6
#define motor_pin1A 7
#define motor_pin1B 5
#else
#define motor_pin0A 6
#define motor_pin0B 8
#define motor_pin1A 5
#define motor_pin1B 7
#endif

#define CMD_STOP   -1
#define CMD_UP     1
#define CMD_DOWN   2
#define CMD_LEFT   3
#define CMD_RIGHT  4

#define RIGHT  1
#define LEFT   0

Motor MotorLeft(motor_pin0A, motor_pin0B);
Motor MotorRight(motor_pin1A, motor_pin1B);

bool leftFlag = false;
bool rightFlag = false;
bool back;
int16_t leftSpeed = 0;
int16_t rightSpeed = 0;
uint32_t micTime = 0;
uint32_t trackTime = 0, trackTime1 = 0;

bool fback = true;

void motorUpdate(int16_t speed1, int16_t speed2)
{
  MotorLeft.Driver(speed1);
  MotorRight.Driver(speed2);
}

void motorCoast()
{
  leftSpeed = 0;
  rightSpeed = 0;
}

void bleControl(int8_t bleCmd)
{
  switch (bleCmd)
  {
    case CMD_UP:
      setAllLed(COLOR_COLD);
      leftSpeed = MAX_THROTTLE;
      rightSpeed = MAX_THROTTLE;
      fback = true;
      break;
    case CMD_DOWN:
      setAllLed(COLOR_RED);
      leftSpeed = -(255 - MAX_THROTTLE);
      rightSpeed = -(255 - MAX_THROTTLE);
      fback = false;
      break;
    case CMD_LEFT:
      if (fback)
      {
        leftSpeed = MAX_THROTTLE - STEER;
        rightSpeed = MAX_THROTTLE + STEER;
        setLed(COLOR_NONE, RIGHT);
      }
      else
      {
        leftSpeed = -MAX_THROTTLE + STEER;
        rightSpeed = -MAX_THROTTLE +- STEER;
        setLed(COLOR_NONE, RIGHT);
      }
      ledBlink(500, COLOR_RED, LEFT);
      break;
    case CMD_RIGHT:
      if (fback)
      {
        leftSpeed = MAX_THROTTLE + STEER;
        rightSpeed = MAX_THROTTLE - STEER;
        setLed(COLOR_NONE, LEFT);
      }
      else
      {
        leftSpeed = -MAX_THROTTLE - STEER;
        rightSpeed = -MAX_THROTTLE + STEER;
        setLed(COLOR_NONE, LEFT);
      }
      ledBlink(500, COLOR_RED, RIGHT);
      break;
    case CMD_STOP:
      if (leftSpeed != 0 || rightSpeed != 0)
      {
        setAllLed(COLOR_NONE);
        leftSpeed = 0;
        rightSpeed = 0;
      }
      break;
    default:
      break;
  }
}

void trackControl(uint16_t trackVal1, uint16_t trackVal2)
{
  if (trackVal1 < TRACK_THRESHOLD && trackVal2 < TRACK_THRESHOLD)
  {
    leftSpeed = MAX_THROTTLE - tackforward;
    rightSpeed = MAX_THROTTLE - tackforward;
    setAllLed(COLOR_COLD);
  }
  else if (trackVal1 < TRACK_THRESHOLD && trackVal2 > TRACK_THRESHOLD)
  {
    leftSpeed = MAX_THROTTLE - tackforward + tacksteer;
    rightSpeed = MAX_THROTTLE - tackforward - tacksteer;
    setLed(COLOR_WARM, LEFT);
    setLed(COLOR_NONE, RIGHT);
  }
  else if (trackVal1 > TRACK_THRESHOLD && trackVal2 < TRACK_THRESHOLD)
  {
    leftSpeed = MAX_THROTTLE - tackforward - tacksteer;
    rightSpeed = MAX_THROTTLE - tackforward + tacksteer;
    setLed(COLOR_WARM, RIGHT);
    setLed(COLOR_NONE, LEFT);
  }
  else if (trackVal1 > TRACK_THRESHOLD && trackVal2 > TRACK_THRESHOLD)
  {
    setAllLed(COLOR_NONE);
    leftSpeed = 0;
    rightSpeed = 0;
  }
}

void micControl(long micVal, long trackVal1, long trackVal2) {
  if (micVal > 200 && micTime == 0)
    micTime = millis() + 6000;
  else if (millis() < micTime && micTime > 0)
  {
    trackVal1 = constrain(trackVal1, 800, 1023);
    trackVal1 = (map(trackVal1, 800, 1023, (-255), 255));
    trackVal2 = constrain(trackVal2, 850, 1023);
    trackVal2 = (map(trackVal2, 800, 1023, (-255), 255));
    leftSpeed = trackVal2;
    rightSpeed = trackVal1;
    if (trackVal1 < 40 && trackVal2 < 40) {
      if (back) {
        trackTime = millis();
        back = false;
      }
      trackTime1 = millis();
      if (trackTime1 - trackTime > 800) {
        setAllLed(COLOR_RED);
        motorUpdate(-150, -150);
        delay(1000);
        motorUpdate(150, -150);
        delay(300);
      }
    } else {
      setAllLed(COLOR_COLD);
      back = true;
      trackTime1 = trackTime;
    }
  } else {
    setAllLed(COLOR_NONE);
    leftSpeed = 0;
    rightSpeed = 0;
    micTime = 0;
  }
}

void soundmode(uint8_t _mode)
{
  if (_mode == 0)
  {
    tone(PIN_BUZZER, 523);
    setAllLed(COLOR_RED);
    delay(100);
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    delay(100);
    tone(PIN_BUZZER, 523);
    setAllLed(COLOR_RED);
    delay(100);
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    delay(100);
    tone(PIN_BUZZER, 523);
    setAllLed(COLOR_RED);
    delay(100);
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    delay(100);
  }
  if (_mode == 1)
  {
    tone(PIN_BUZZER, 523);
    setAllLed(COLOR_RED);
    delay(300);
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    delay(300);
  }
  else if (_mode == 2)
  {
    tone(PIN_BUZZER, 523);
    setAllLed(COLOR_GREEN);
    delay(300);
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    delay(300);
    tone(PIN_BUZZER, 590);
    setAllLed(COLOR_GREEN);
    delay(300);
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    delay(300);
  }
  else if (_mode == 3)
  {
    tone(PIN_BUZZER, 500);
    setAllLed(COLOR_BLUE);
    delay(300);
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    delay(300);
    tone(PIN_BUZZER, 590);
    setAllLed(COLOR_BLUE);
    delay(300);
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    delay(300);
    tone(PIN_BUZZER, 660);
    setAllLed(COLOR_BLUE);
    delay(300);
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    delay(300);
  }
}
