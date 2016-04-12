/* maker remote version3 by chiangmai maker club */

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>

#define buffer_size 100
#define period_udp 10   // 100Hz


#define _key 0xff  // changing for safety 
#define _port 12345

int8_t Channel[6] = {0};


/* Set these to your desired credentials. */
const char *ssid = "ESP_CF";
const char *password = "12345678";

IPAddress local_ip(192, 168, 5, 1);

unsigned int localPort = _port;
int count = 0;
int ch1, ch2, ch3, ch4;
int _ch1, _ch2, _ch3, _ch4;

unsigned long time_now, time_prev_udp;

int8_t packetBuffer[buffer_size] = {0}; //buffer to hold incoming and outgoing packets
WiFiUDP udp;

void Read_adc();
void Read_adc_offset();
void Sent_UDP();
void d1_toggle();

void setup() 
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output

 delay(1000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  udp.begin(localPort);

  time_now = millis();
  time_prev_udp = time_now;

}

void loop() 
{
  time_now = millis();


  if (time_now - time_prev_udp >= period_udp)
  {
    time_prev_udp = time_now;
    Read_adc();
    Sent_UDP();
  }

  delay(1);
}
void Sent_UDP()
{
  packetBuffer[0] = _key;
  packetBuffer[1] = (int8_t)Channel[0] ;
  packetBuffer[2] = (int8_t)Channel[1] ;
  packetBuffer[3] = (int8_t)Channel[2] ;
  packetBuffer[4] = (int8_t)Channel[3] ;
  packetBuffer[5] = (int8_t)Channel[4] ;
  packetBuffer[6] = (int8_t)Channel[5] ;

  udp.beginPacket(local_ip, localPort);
  udp.write((char*)packetBuffer,7);
  udp.endPacket();


  memset(packetBuffer, 0, buffer_size);
}

void Read_adc()
{
  Wire.requestFrom(8, 6);    // request 6 bytes from slave device #8
  for (int i = 0; i < 6; i++)
  {
    Channel[i] = Wire.read(); // receive a byte as character
    Serial.print(Channel[i]);         // print the character
  }
  Serial.println("");

// limit

if (Channel[0] > 100) Channel[0] = 100;
if (Channel[0] < -100) Channel[0] = -100;

if (Channel[1] > 100) Channel[1] = 100;
if (Channel[1] < -100) Channel[1] = -100;

if (Channel[2] > 100) Channel[2] = 100;
if (Channel[2] < 0) Channel[2] = 0;

if (Channel[3] > 100) Channel[3] = 100;
if (Channel[3] < -100) Channel[3] = -100;

if (Channel[4] > 100) Channel[4] = 100;
if (Channel[4] < -100) Channel[4] = -100;

if (Channel[5] > 100) Channel[5] = 100;
if (Channel[5] < -100) Channel[5] = -100;

if (Channel[6] > 100) Channel[6] = 100;
if (Channel[6] < -100) Channel[6] = -100;
}
