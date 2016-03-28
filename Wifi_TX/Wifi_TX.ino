
#include <Wire.h>

int8_t Channel[6] = {0};

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
}

void loop() {
  Wire.requestFrom(8, 6);    // request 6 bytes from slave device #8


  for (int i = 0; i < 6; i++)
  {
    Channel[i] = Wire.read(); // receive a byte as character
    Serial.print(Channel[i]);         // print the character
  }
  Serial.println("");


  delay(10);
}
