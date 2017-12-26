#include <SPI.h>

#define SS 10

#define CMD_LEDON '0'
#define CMD_LEDOFF '1'

void setup() {
  Serial.begin(115200);
  SPI.begin();

  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV128); // 128kKz
}

inline byte SPItransfer(byte cmd) {
  byte d = SPI.transfer(cmd);
  delayMicroseconds(150);
  return d;
}

float getFloat(byte cmd) {
  union {
    float f;
    byte b[4];
  };
  SPItransfer(cmd);
  b[0] = SPItransfer(0);
  b[1] = SPItransfer(0);
  b[2] = SPItransfer(0);
  b[3] = SPItransfer(0);
  return f;
}

long getLong(byte cmd) {
  SPItransfer(cmd);
  byte a = SPItransfer(0);
  byte b = SPItransfer(0);
  byte c = SPItransfer(0);
  byte d = SPItransfer(0);
  return a | (b << 8) | (c << 16) | (d << 24);
}

int getInt(byte cmd) {
  SPItransfer(cmd);
  byte a = SPItransfer(0);
  byte b = SPItransfer(0);
  int i = a | (b << 8);
  return i;
}

byte getByte(byte cmd) {
  SPItransfer(cmd);
  byte b = SPItransfer(0);
  return b;
}

void loop() {
  Serial.print(getByte('c'));
  Serial.print(" ");
  Serial.print(getInt('i'));
  Serial.print(" ");
  Serial.print(getLong('l'));
  Serial.print(" ");
  Serial.println(getFloat('f'));
  delay(250);
  SPI.transfer(CMD_LEDON);
  delay(500);
  SPI.transfer(CMD_LEDOFF);
  delay(250);
}
