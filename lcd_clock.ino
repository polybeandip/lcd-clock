#include <LiquidCrystal.h>
#include <WiFi.h>
#include <time.h>

#define MSG_DISPLAY_MS 2000
#define WIFI_SSID "Verizon_6XVDQ4"
#define WIFI_PASS "ohm8screed2brim"

const int button = 32;
LiquidCrystal lcd(13, 33, 25, 26, 27, 14);

bool was_pressed = false;
unsigned long button_press_ms = 0;

int minute = -1;
bool block_time = false;

void setup() {
  pinMode(button, INPUT_PULLUP);
  lcd.begin(16, 2);
  lcd.clear();
  
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  // Set timezone to America/New_York
  configTime(0, 0, "pool.ntp.org");
  setenv("TZ", "EST5EDT,M3.2.0,M11.1.0", 1);
  tzset();

  lcd.setCursor(0, 0);
  lcd.print("CLOCK");
}

void loop() {
  // Print the time?
  if (!block_time) {
    struct tm now;
    getLocalTime(&now);
    if (was_pressed || (minute != now.tm_min)) {
      int hour = now.tm_hour;
      minute = now.tm_min;

      // Print the time!
      char time[6];
      time[5] = '\0';
      time[0] = '0' + ((hour / 10) % 10);
      time[1] = '0' + (hour % 10);
      time[2] = ':';
      time[3] = '0' + ((minute / 10) % 10);
      time[4] = '0' + (minute % 10);

      lcd.clear();

      lcd.setCursor(5, 0);
      lcd.print(time);
      lcd.setCursor(0, 1);
      lcd.print("New York");

      was_pressed = false;
    }
  }

  // Check for button press every MSG_DISPLAY_MS
  if (millis() - button_press_ms >= MSG_DISPLAY_MS) {
    int pressed = !digitalRead(button);
    block_time = pressed;
    if (pressed) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Button not work!");
      button_press_ms = millis();
      was_pressed = true;
    }
  }
}
