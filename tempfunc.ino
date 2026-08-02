#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// OLED Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DS18B20 Configuration
#define ONE_WIRE_BUS 4 // Pin where DS18B20 Data wire is connected
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup()
{
    Serial.begin(115200);

    // Start OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("OLED connection failed"));
        for (;;)
            ;
    }

    // Start Sensor
    sensors.begin();

    // Initial Splash Screen
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(40, 15);
    display.println("Fish Farm");
    display.setCursor(15, 30);
    display.println("Monitoring System");
    display.display();
    delay(2000); // Show branding for 2 seconds
}

void loop()
{
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);

    // --- Serial Monitor ---
    Serial.print("Fish Farm Monitoring System Reading: ");
    if (tempC == -127.00)
    {
        Serial.println("Error: Sensor Disconnected");
    }
    else
    {
        Serial.print(tempC);
        Serial.println(" C");
    }

    // --- OLED Display ---
    display.clearDisplay();

    // Top Branding
    display.setTextSize(1);
    display.setCursor(50, 5);
    display.println("FFMS");
    display.drawFastHLine(0, 16, 128, WHITE); // Simple clean separator line

    // Main Temperature Value
    if (tempC == -127.00)
    {
        display.setTextSize(1);
        display.setCursor(20, 35);
        display.println("SENSOR ERROR!");
    }
    else
    {
        display.setTextSize(1);
        display.setCursor(30, 25);
        display.println("TEMPERATURE");

        display.setTextSize(2);
        display.setCursor(25, 42);
        display.print(tempC);
        display.setTextSize(1);
        display.print(" o"); // Degree symbol
        display.setTextSize(2);
        display.print("C");
    }

    display.display();
    delay(1500); // Refresh every 0.5 seconds
}