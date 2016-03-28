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
      Offset[i] = (analogRead(Pin[i]) >> 2);
    }
    delay(10);
    if (Offset[2] < 60) break;
  }
}

void loop()
{
  for (int i = 0; i < 6; i++)
  {
    Channel[i] = (analogRead(Pin[i]) >> 2) - Offset[i]; // 8bit
  }
#ifdef debug
  for (int i = 0; i < 6; i++)
  {
    Serial.print("  CH ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(Channel[i]);
  }
  Serial.println("");
  delay(90);
#endif
  delay(10);
}

void requestEvent()
{
  digitalWrite(LED, HIGH);
    Wire.write(Channel,6); // respond with message of 6 bytes
  digitalWrite(LED, LOW);
}
