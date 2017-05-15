#include <EEPROM.h>

//EEPROM配置
#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

uint8_t mode = 1;
int8_t key = -1;
uint8_t tackforward = 40;
uint8_t tacksteer = 30;

struct config_type
{
  uint8_t EEPROMmode;
  uint8_t EEPROMforward;
  uint8_t EEPROMsteer;
};

void read_eeprom() {
  config_type config_readback;
  EEPROM_read(1, config_readback);
  mode = config_readback.EEPROMmode;
  tackforward = config_readback.EEPROMforward;
  tacksteer = config_readback.EEPROMsteer;
}

void write_eeprom() {
  config_type config;                  // 定义结构变量config，并定义config的内容
  config.EEPROMmode = mode;
  config.EEPROMforward = tackforward;
  config.EEPROMsteer = tacksteer;
  EEPROM_write(1, config);         // 变量config存储到EEPROM，地址0写入
}
