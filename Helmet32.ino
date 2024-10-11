#include <TinyGPS++.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define SCREEN_ADDRESS 0x3C

#define RXD2 16
#define TXD2 17
#define GPS_BAUD 9600

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
HardwareSerial gpsSerial(2);
TinyGPSPlus gps;

void changeFrequency() {
    byte packet[] = {
        0xB5, // 
        0x62, // 
        0x06, // 
        0x08, // 
        0x06, // length
        0x00, // 
        0xFF, // measRate, hex 64 = dec 100 ms
        0x01, // 
        0x01, // navRate, always =1
        0x00, // 
        0x01, // timeRef, stick to GPS time (=1)
        0x00, // 
        0x7A, // CK_A
        0x12, // CK_B
    };
    sendPacket(packet, sizeof(packet));
}

void sendPacket(byte *packet, byte len) {
  for (byte i = 0; i < len; i++) {
    gpsSerial.write(packet[i]);
  }
}

String getStringPaddedForTime(int value) {
  if (value == -1) {
    return "--";
  }
  String valueText = String(value);
  if (value < 10) {
    valueText = "0" + valueText;
  }
  return valueText;
}

String intAsText(int value) {
  if (value == -1) {
    return "--";
  }
  return String(value);
}
void displayPrincipalScreen(int satellites, int hdop, int speed, int hour, int minute) {

  // Speed
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(5);
  display.println(intAsText(speed));
  // HOUR
  String hourText = getStringPaddedForTime(hour);
  String minuteText = getStringPaddedForTime(minute);
  display.setTextSize(2);
  display.setCursor(100,0);
  display.println(hourText);
  display.setCursor(100,18);
  display.println(minuteText);
    
  display.drawLine(70, 0, 70, 31, SSD1306_WHITE);

  display.drawLine(82, 2, 84, 2, SSD1306_WHITE);
  display.drawLine(82, 3, 84, 3, SSD1306_WHITE);    
  display.drawLine(82, 4, 84, 4, SSD1306_WHITE);
  display.drawLine(80, 5, 80, 27, SSD1306_WHITE);
  display.drawLine(80, 5, 80, 5, SSD1306_WHITE);
  display.drawLine(86, 5, 86, 27, SSD1306_WHITE);
  display.drawLine(80, 27, 86, 27, SSD1306_WHITE);
  for (int i = 1; i < 6; i++) {
    display.drawLine(80 + i, 15, 80 + i, display.height() - 5, SSD1306_WHITE);
  }

  display.setTextSize(1);
  display.setCursor(61, 3);
  
  display.println(intAsText(satellites));
  display.drawLine(55, 0, 55, 31, SSD1306_WHITE);

  // hdop

  display.display();
}

int getSpeed() {
  int speed = int(trunc(gps.speed.kmph()));
  if (speed < 3) {
      speed = 0;
  }
  return speed;
}

void setup() {
  Serial.begin(115200);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    // Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  gpsSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
  //changeFrequency();
  //delay(100);
  // gpsSerial.flush(); 
  // gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
  // Serial.println("Serial 2 started at 9600 baud rate");
}

int speed = -1;
int hour = -1;
int minute = -1;
int satellites = -1;
int hdop = -1;

void loop() {
  // Serial.println(gpsSerial.available());
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }
  if (gps.speed.isUpdated()) {
    speed = gps.speed.isValid() ? getSpeed() : -1;
    Serial.println("(" + String(gps.satellites.value()) + " sats, hdop : " + String(gps.hdop.value()) + ") " + String(gps.time.hour()) + ":" + String(gps.time.minute()) + " => " + String(gps.speed.kmph()));
  }
  if (gps.time.isUpdated()) {
    hour = gps.time.isValid() ? gps.time.hour() : -1;
    minute = gps.time.isValid() ? gps.time.minute() : -1;
  }
  if (gps.satellites.isUpdated()) {    
    satellites = gps.satellites.value();
  }
  if (gps.hdop.isUpdated()) {
    hdop = gps.hdop.value();
  }
  displayPrincipalScreen(satellites, hdop, speed, hour/*((hour + 2) % 24)*/, minute);
}
