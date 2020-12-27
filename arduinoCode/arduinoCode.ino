//author: chenliang @ Youtu Lab, Tencent
#include <DataLinkArduino.h>
#include <pb_decode.h>
#include <math.h>
#include "Syn6288.h"
#include <SoftwareSerial.h>
//实例化软串口
SoftwareSerial mySerial(2, 3); // RX, TX

YtDataLink dl(new YtSerialPortArduino(&Serial));

Syn6288 syn;
uint8_t digits[]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};   //0,1,2,3,4,5,6,7,8,9
void setup()
{
    Serial.begin(9600);
    mySerial.begin(9600);
}

#define PAIRS 4
#define MAX_STR_SIZE 20
const char m[PAIRS][2][MAX_STR_SIZE] =
    { {"沪EXZ390", "222"},
      {"沪GD7055", "666"},
      {"沪A31358D", "770"},
      {"黑AF6655", "333"}
    };

bool compare(const char* src, const char* tar){
  src += 2;
  int count = 0;
  while(*tar != '\0' && *tar != *src) tar++;
  while(*tar++ == *src++){
      count++;
  }
  return count > 3;
}

void sound(char *p){
    uint8_t *prt = (uint8_t *)malloc(strlen(p) + 2);
    uint8_t lu[] = {0xC2, 0xB7};
    Serial.println(strlen(p));
    for(int i=0;i<strlen(p);i++){
      prt[i] = digits[p[i] - 48];
    }
    memcpy(prt + strlen(p), lu, 2);
    
    syn.play(prt,strlen(p) + 2,1, mySerial);
    delay(1000);
}

void loop()
{ 
    const uint8_t *message = dl.recvRunOnce();
    if (message)
    {
        YtVisionSeedResultTypeString plate = {.conf = 0, .p = nullptr};
        if (!YtDataLink::getResult(message, &plate, {20, 0, 21}))
        {
            Serial.println("no message");
        }
        else{
          Serial.print(plate.conf);
          Serial.print(" ");
          Serial.println(plate.p);
  
          for(int i=0;i < PAIRS; i++){
            if(compare(m[i][0], plate.p)){
              Serial.print("Detect: ");
              Serial.println(m[i][1]);
              sound(m[i][1]);
            }
          }
        }
    }
}
