#include <Wire.h>

void setup() {
  Wire.begin(); // use Wire.begin(SDA_PIN, SCL_PIN); for custom I2C pins
  Serial.begin(115200);
  Serial.println("\nI2C Scanner Running...");
}

void loop() {
  byte error, address;
  int devices = 0;

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
      devices++;
    }
  }

  if (devices == 0)
    Serial.println("No I2C devices found.\n");
  else
    Serial.println("Scan complete.\n");

  delay(15000); // Wait 15 seconds before the next scan
}