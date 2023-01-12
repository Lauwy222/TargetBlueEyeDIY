//Libraries
#include <Arduino.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

// Pin definitions
const int MY_MOSI = 27;
const int SCLK = 5;
const int CS = 18;
const int DI0 = 26;
const int RST = 23;
const int MY_MISO = 19;

//Screen
Adafruit_SSD1306 display(128, 64, &Wire);

//Frequency Sets
long minfreq = 454E6;
long maxfreq = 470E6;
long stepfreq = 1E5;

void setup() {
  //Display startup
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  //Starting up
    Serial.begin(115200);
    while (!Serial);
    Serial.println("LoRa Scanner");
    display.println("LoRa Scanner V1.0");
    
    display.println("");
    
    //Frequency range
      //Display
    display.println("Frequency range: ");
    display.print(maxfreq/1000000);
    display.print("MHz - ");
    display.print(minfreq/1000000);
    display.println("MHz");
    display.display();
      //Serial
    Serial.println("Frequency range: ");
    Serial.print(maxfreq/1000000);
    Serial.print("MHz - ");
    Serial.print(minfreq/1000000);
    Serial.println("MHz");

    //Step frequency
      //Display
    display.println("");
    display.println("Step Freq: ");
    display.print(stepfreq/1000);
    display.println("GHz");
    display.display();
      //Serial
    Serial.println("Step Freq: ");
    Serial.print(stepfreq/1000);
    Serial.println("GHz");
    delay(5000);

    //Setpins
      LoRa.setPins(CS, RST, DI0);
    //Error
    if (!LoRa.begin(minfreq)) {
        display.clearDisplay();
        display.setCursor(0,0);
        Serial.println("Starting LoRa failed!");
        display.println("Starting LoRa failed!");
        display.display();
        while (1);
    }
    //SPI pins
    SPI.begin(SCLK, MY_MISO, MY_MOSI);
}

void loop() {
   // for (long frequency = minfreq; frequency < maxfreq; frequency += stepfreq) {
        LoRa.setFrequency(frequency);
        int packetSize = LoRa.parsePacket();
        if (packetSize) {
            Serial.print("Frequency: ");
            Serial.print(frequency);
            Serial.print("Hz  |  RSSI: ");
            Serial.print(LoRa.packetRssi());
            Serial.print("dBm  |  SNR: ");
            Serial.print(LoRa.packetSnr());
            Serial.println("dB");
        }
    }
    delay(1000);
}
