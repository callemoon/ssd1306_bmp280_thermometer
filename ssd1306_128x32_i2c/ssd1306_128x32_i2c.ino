// Thermometer for Raspberry Pi Pico using SSD1306 OLED 128*32 display and BMP280 temp sensor
// Connect bmp280 to I2C0 on pin 16/17
// Connect ssd1306 to I2C1 on pin 18/19

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET);

Adafruit_BMP280 bmp; // I2C

void setup() {
  Serial.begin(9600);

  // BMP280 temp sensor
  Wire.setSDA(16);
  Wire.setSCL(17);
  Wire.begin();

  // SSD1306 display
  Wire1.setSDA(18);
  Wire1.setSCL(19);

  if(!bmp.begin(BMP280_ADDRESS_ALT))
  {
    Serial.println(F("BMP280 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}

void loop() {
  float temp = bmp.readTemperature();
  float pressure = bmp.readPressure(); 

  display.clearDisplay();
  display.setTextSize(3); // Draw 3X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(temp);

  display.setTextSize(1);
  display.setCursor(10, 24);
  display.println(pressure);

  display.display(); 
  delay(500);
}
