
/* Create a WiFi access point. */
//#define DEBUG
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define _key 0xff  // changing for safety 
#define _port 12345

#define Number_of_Channel 6



#define buffer_size 20

#define period_udp 1
#define period_led 1000


/* Set these to your desired credentials. */
const char *ssid = "ESP_CF";
const char *password = "12345678";

unsigned int localPort = _port;
unsigned long time_now, time_prev_udp, time_prev_led;
int cb = 1;



char packetBuffer[buffer_size] = {0}; //buffer to hold incoming and outgoing packets
WiFiUDP udp;
int count = 0;

IPAddress local_ip(192, 168, 5, 1);
IPAddress gateway(192, 168, 5, 1);
IPAddress subnet(255, 255, 255, 255);

uint16_t rcData[] = {1500, 1500 , 1000 , 1500 , 1500 , 1500, 1500, 1500};

void Read_UDP(void);
void send_msp(void);
void send_serial(void);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  /* You can remove the password parameter if you want the AP to be open. */
  delay(500);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);

  delay(500);
  IPAddress myIP = WiFi.softAPIP();
  udp.begin(localPort);

  delay(3000);
  time_now = millis();
  time_prev_udp = time_now;
  time_prev_led = time_now;
}

void loop()
{
  // put your main code here, to run repeatedly:

  time_now = millis();

  if (time_now - time_prev_udp >= period_udp)
  {
    time_prev_udp = time_now;
    Read_UDP();
  }

  delay(1);
}



void Read_UDP(void)
{
  cb = udp.parsePacket();
  if (cb)
  {
    memset(packetBuffer, 0, buffer_size); // clear mem
    udp.read(packetBuffer, cb); // read the packet into the buffer
    for (int i = 0; i < Number_of_Channel; i++)
    {

      if (i == 4 || i == 5)
      {
        rcData[i] = 1000 + (int8_t)packetBuffer[i + 1] * 10 ;  // switch
      } else if (i == 2)       //  swap ch 3 <=> ch 4  //Futaba <> CF
      {
        rcData[3] = 1000 + (int8_t)packetBuffer[i + 1] * 10 ;  // ch3
      } else if (i == 3)       //  swap ch 3 <=> ch 4  //Futaba <> CF
      {
        rcData[2] = 1500 + (int8_t)packetBuffer[i + 1] * 5 ;   // ch4
      } else {
        rcData[i] = 1500 + (int8_t)packetBuffer[i + 1] * 5 ;
      }
    }
    // rcData[i] = 1500 +  (int16_t)packetBuffer[i*2]<<8 | (int16_t)packetBuffer[(i*2) + 1] ;
    if (packetBuffer[0] == _key)
    {
#ifdef DEBUG
      send_serial();
#else
      send_msp();
#endif
    }
  }
}


void send_msp(void)
{
  const uint8_t size_data = 16;
  const uint8_t type_data = 200;

  uint8_t checksum = 0;
  uint8_t* data =  ((uint8_t*)rcData);
  // Send header
  Serial.write(0x24);
  Serial.write(0x4D);
  Serial.write(0x3C);

  // Send size
  Serial.write(size_data);
  checksum ^= size_data;

  // Send data type (MSP_SET_RAW_RC = 200)
  Serial.write(type_data);
  checksum ^= type_data;

  // Send the data
  for (int i = 0; i < size_data; i++)
  {
    Serial.write(data[i]);
    checksum ^= data[i];
  }
  // Send checksum
  Serial.write(checksum);

}

void send_serial(void)
{

  const uint8_t size_data = 12;
  const uint8_t type_data = 200;

  uint8_t checksum = 0;
  uint8_t* data =  ((uint8_t*)rcData);
  // Send header
  Serial.print(0x24, HEX);   Serial.print("  ");
  Serial.print(0x4D, HEX);   Serial.print("  ");
  Serial.print(0x3C, HEX);   Serial.print("  ");

  // Send size
  Serial.print(size_data, HEX);   Serial.print("  ");
  checksum ^= size_data;

  // Send data type (MSP_SET_RAW_RC = 200)
  Serial.print(type_data, HEX);   Serial.print("  ");
  checksum ^= type_data;

  // Send the data
  for (int i = 0; i < size_data / 2; i++)
  {
    Serial.print(rcData[i]);   Serial.print("  ");
    checksum ^= data[i];
  }
  // Send checksum
  Serial.println(checksum, HEX);

}
