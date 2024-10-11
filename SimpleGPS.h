#ifndef SIMPLEGPS_H
#define SIMPLEGPS_H
#include <Arduino.h>
#include <TinyGPS++.h>

#define RXD2 16
#define TXD2 17
#define GPS_BAUD 9600

class SimpleGPS {
  public:
    SimpleGPS();
    bool speedHasChanged() const;
    int getSpeed() const;
    int getHour() const;
    int getMinute() const;
    TinyGPSPlus* getGPS() const;
  private: 
    TinyGPSPlus* gps;
    HardwareSerial* gpsSerial;
    void checkNewData() const;
};

#endif
