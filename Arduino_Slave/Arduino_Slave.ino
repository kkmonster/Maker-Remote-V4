// i2c slave as ADC module update rate ~100Hz
//
// Wasin Wongkum
// 2016

//#define debug

#define LED 13
#define slave_addr 8

#include <Wire.h>
const int8_t Pin[] = {A0, A1, A2, A3, A6, A7};
byte Channel[6] = {0};
int Offset[6] = {0};


void setup() {
  Serial.begin(115200);
  Wire.begin(slave_addr);
  Wire.onRequest(requestEvent);
  pinMode(LED, OUTPUT);
  delay(10);
  while (1)
  {
    for (int i = 0; i < 4; i++)
    {
      Offset[i] = analogRead(Pin[i]);
    }
    delay(10);
    if (Offset[2] < 240) break;
  }
}

void loop()
{
  for (int i = 0; i < 6; i++)
  {
    int tmp = 0;
    if ( i == 2 )
    {
      tmp = (float)(analogRead(Pin[i]) - Offset[i]) / 1.2f; // 8bit
    } else if(i == 4 || i == 5){
      tmp = analogRead(Pin[i])  ; // 8bit
      tmp >>= 1;
      tmp =(float)tmp/1.27f;

    } else { 
      tmp = analogRead(Pin[i]) - Offset[i]; // 8bit
    }

    Channel[i] = (int8_t)(tmp >> 2);
  }

#ifdef debug
  for (int i = 0; i < 6; i++)
  {
    Serial.print("  CH ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print((int8_t)Channel[i]);
  }
  Serial.println("");
  delay(90);
#endif
  delay(10);
}

void requestEvent()
{
  digitalWrite(LED, HIGH);
  Wire.write(Channel, 6); // respond with message of 6 bytes
  digitalWrite(LED, LOW);
}




