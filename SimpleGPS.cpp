#include "SimpleGPS.h"
#include <TinyGPS++.h>

SimpleGPS::SimpleGPS() {
  this->gps = new TinyGPSPlus();
  this->gpsSerial = new HardwareSerial(2);
  gpsSerial->begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
}

void SimpleGPS::checkNewData() const {
  while (gpsSerial->available() > 0) {
    gps->encode(gpsSerial->read());
  }
}

bool SimpleGPS::speedHasChanged() const {
  Serial.println("speedHasChanged");
  checkNewData();
  return gps->speed.isUpdated();
}

int SimpleGPS::getSpeed() const {
  Serial.println("getSpeed");
  int speed = int(trunc(gps->speed.kmph()));
  if (speed < 3) {
      speed = 0;
  }
  return speed;
}

int SimpleGPS::getHour() const {
  Serial.println("getHour");
  return gps->time.hour();
}

int SimpleGPS::getMinute() const {
  Serial.println("getMinute");
  return gps->time.minute();
}

TinyGPSPlus* SimpleGPS::getGPS() const {
  return gps;
}


//void changeFrequency() {
//    byte packet[] = {
//        0xB5, // 
//        0x62, // 
//        0x06, // 
//        0x08, // 
//        0x06, // length
//        0x00, // 
//        0xE8, // measRate, hex 64 = dec 100 ms
//        0x03, // 
//        0x01, // navRate, always =1
//        0x00, // 
//        0x01, // timeRef, stick to GPS time (=1)
//        0x00, // 
//        0x7A, // CK_A
//        0x12, // CK_B
//    };
//    sendPacket(packet, sizeof(packet));
//}
//
//  //gpsSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
//  //changeFrequency();
//  //delay(100);
//  //gpsSerial.flush();
//
//void sendPacket(byte *packet, byte len) {
//  for (byte i = 0; i < len; i++) {
//    Serial2.write(packet[i]); // GPS is HardwareSerial
//  }
//}
