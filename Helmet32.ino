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

void setup()
{
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    // Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  gpsSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void displayPrincipalScreen(int satellites, int hdop, int speed, int hour, int minute)
{
  // Speed
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(5);
  display.println(intAsText(speed));
  // Time
  String hourText = getStringPaddedForTime(hour);
  String minuteText = getStringPaddedForTime(minute);
  display.setTextSize(2);
  display.setCursor(100, 0);
  display.println(hourText);
  display.setCursor(100, 18);
  display.println(minuteText);
  // Battery
  display.drawLine(82, 2, 84, 2, SSD1306_WHITE);
  display.drawLine(82, 3, 84, 3, SSD1306_WHITE);
  display.drawLine(82, 4, 84, 4, SSD1306_WHITE);
  display.drawLine(80, 5, 80, 27, SSD1306_WHITE);
  display.drawLine(80, 5, 80, 5, SSD1306_WHITE);
  display.drawLine(86, 5, 86, 27, SSD1306_WHITE);
  display.drawLine(80, 27, 86, 27, SSD1306_WHITE);
  for (int i = 1; i < 6; i++)
  {
    display.drawLine(80 + i, 15, 80 + i, display.height() - 5, SSD1306_WHITE);
  }
  // Satellites
  display.setTextSize(1);
  display.setCursor(61, 3);
  display.println(intAsText(satellites));
  // Sections
  display.drawLine(70, 0, 70, 31, SSD1306_WHITE);
  display.drawLine(55, 0, 55, 31, SSD1306_WHITE);
  // HDOP
  // TODO
  // Display
  display.display();
}

int getSpeed()
{
  int speed = int(trunc(gps.speed.kmph()));
  if (speed < 3)
  {
    speed = 0;
  }
  return speed;
}

int speed = -1;
int hour = -1;
int minute = -1;
int satellites = -1;
int hdop = -1;

void loop()
{
  while (gpsSerial.available() > 0)
  {
    gps.encode(gpsSerial.read());
  }
  if (gps.speed.isUpdated())
  {
    speed = gps.speed.isValid() ? getSpeed() : -1;
    Serial.println("(" + String(gps.satellites.value()) + " sats, hdop : " + String(gps.hdop.value()) + ") " + String(gps.time.hour()) + ":" + String(gps.time.minute()) + " => " + String(gps.speed.kmph()));
  }
  if (gps.time.isUpdated())
  {
    hour = gps.time.isValid() ? gps.time.hour() : -1;
    minute = gps.time.isValid() ? gps.time.minute() : -1;
  }
  if (gps.satellites.isUpdated())
  {
    satellites = gps.satellites.value();
  }
  if (gps.hdop.isUpdated())
  {
    hdop = gps.hdop.value();
  }
  displayPrincipalScreen(satellites, hdop, speed, (hour == -1 ? -1 : (hour + 2) % 24), minute);
}
