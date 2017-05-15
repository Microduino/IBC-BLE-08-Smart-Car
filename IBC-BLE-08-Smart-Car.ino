#include "userDef.h"
#include "colorLed.h"
#include "eeprom.h"
#include "motor.h"
#include "carBLEJson.h"

uint16_t lineValA;
uint16_t lineValB;
uint16_t micVal;

void setup() {
  Serial.begin(57600);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LINEA, INPUT_PULLUP);
  pinMode(PIN_LINEB, INPUT_PULLUP);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);

  MotorLeft.Fix(1.1);
  MotorRight.Fix(1);

  read_eeprom();
  if (mode < 0 || mode > 3 || tackforward < 20 || tackforward > 60 || tacksteer < 20 || tacksteer > 60)
  {
    mode = 1;
    tackforward = 40;
    tacksteer = 30;
  }

#if DEBUG
  Serial.println("--------------");
  Serial.print("MODE DATA:");
  Serial.println(mode);
  Serial.print("KEY DATA:");
  Serial.println(key);
  Serial.print("TACKF DATA:");
  Serial.println(tackforward);
  Serial.print("TACKS DATA:");
  Serial.println(tacksteer);
  Serial.println("--------------");
#endif

  for (int i = 0; i < 10; i++)
  {
    setAllLed(i);
    delay(150);   //每个颜色持续时间
  }
}

void loop() {
  micVal = analogRead(PIN_MIC);
  lineValA = analogRead(PIN_LINEA);
  lineValB = analogRead(PIN_LINEB);
  //  Serial.print("MIC Val:");
  //  Serial.print(micVal);
  //  Serial.print("\tLINEA Val:");
  //  Serial.print(lineValA);
  //  Serial.print("\tLINEB Val:");
  //  Serial.println(lineValB);
  Carblejson_data();
  switch (mode)
  {
    case MODE_A:
      bleControl(key);
      break;
    case MODE_B:
      trackControl(lineValA, lineValB);
      break;
    case MODE_C:
      micControl(micVal, lineValA, lineValB);
      break;
    default:
      motorCoast();
      break;
  }
  motorUpdate(leftSpeed, rightSpeed);
}
