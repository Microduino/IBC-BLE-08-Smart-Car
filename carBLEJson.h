#include <aJSON.h>
aJsonStream serial_stream(&Serial);

String modeS = "";
String keyS = "";
String tackfS = "";
String tacksS = "";
String projtctid = "";

String ID = "{\"ID\":\"590bc5e87bb3e300016e2da4\"}";

#define  MODE_OFF     0
#define  MODE_A       1
#define  MODE_B       2
#define  MODE_C       3

//{"ID":"project"}
void IDupload()
{
  Serial.print(ID.substring(0, 17));
  delay(30);
  Serial.println(ID.substring(17, 33));
  delay(30);
}

void Ctrldata(String _st, int _data)
{
  String send_data;
  int number;
  send_data = "{\"";
  send_data += _st;
  send_data += "\":\"";
  send_data += _data;
  send_data += "\"\}";
  number = send_data.length() / 17;

  if (number == 0)
  {
    Serial.println(send_data);
    delay(30);
  }
  else
  {
    while (number >= 0)
    {
      Serial.print(send_data.substring(0, 17));
      send_data = send_data.substring(17, send_data.length());
      delay(30);
      number--;
    }
    Serial.print("\n");
  }
}


//{"mode":"1"}
//{"joystick":"1"}
//{"tackforward":"40"}
//{"tacksteer":"30"}
void ComExecution(aJsonObject *msg)
{
  aJsonObject *IDval = aJson.getObjectItem(msg, "ID");
  if (IDval) {
    projtctid = IDval->valuestring;
    if (projtctid == "project")
    {
      IDupload();
      Ctrldata("trackForward", tackforward );
      Ctrldata("trackSteer", tacksteer);
    }
  }
  else {
#if DEBUG
    Serial.println("no ID data");
#endif
  }

  aJsonObject *modeval = aJson.getObjectItem(msg, "mode");
  if (modeval) {
    modeS  = modeval->valuestring;
    if (modeS != "-1")
    {
      mode = modeS.toInt();
      write_eeprom();
      soundmode(mode);
    }
  }
  else {
#if DEBUG
    Serial.println("no mode data");
#endif
  }
  aJsonObject *keyval = aJson.getObjectItem(msg, "key");
  if (keyval) {
    keyS  = keyval->valuestring;
    key = keyS.toInt();
    write_eeprom();
  }
  else {
#if DEBUG
    Serial.println("no key data");
#endif
  }
  aJsonObject *tackfval = aJson.getObjectItem(msg, "trackForward");
  if (tackfval) {
    tackfS  = tackfval->valuestring;
    tackforward = tackfS.toInt();
    tackforward = 80 - tackforward;
    setAllLed(COLOR_GREEN);
    delay(100);
    setAllLed(COLOR_NONE);
    write_eeprom();
  }
  else {
#if DEBUG
    Serial.println("no tackforward data");
#endif
  }
  aJsonObject *tacksval = aJson.getObjectItem(msg, "trackSteer");
  if (tacksval) {
    tacksS  = tacksval->valuestring;
    tacksteer = tacksS.toInt();
    setAllLed(COLOR_GREEN);
    delay(100);
    setAllLed(COLOR_NONE);
    write_eeprom();
  }
  else {
#if DEBUG
    Serial.println("no tacksteer data");
#endif
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
}

void Carblejson_data()
{
  if (serial_stream.available()) {
    serial_stream.skip();
  }
  if (serial_stream.available()) {
    aJsonObject *msg = aJson.parse(&serial_stream);
    ComExecution(msg);
    aJson.deleteItem(msg);
  }
}
