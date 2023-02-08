#include <SPI.h>
#include <LoRa.h>

// Pin definitions
const int MY_MOSI = 27;
const int SCLK = 5;
const int CS = 18;
const int DI0 = 26;
const int RST = 23;
const int MY_MISO = 19;

const double PATH_LOSS_EXPONENT = 2; // path loss exponent
const double TRANSMITTED_POWER = 14;  // transmitted power in dBm
const double OFFSET = 1e3; // 1 kHz offset
const double START_FREQ = 433e6; // 433 MHz start frequency
const double END_FREQ = 438e6; // 438 MHz end frequency
const double STEP = 1e6; // 1 MHz step

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  // Set the pin connections for the LoRa module
  LoRa.setPins(CS, RST, DI0);
  if (!LoRa.begin(START_FREQ)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // Set the pin connections for the SPI bus
  SPI.begin(SCLK, MY_MISO, MY_MOSI);
}

void loop() {
  for (double freq = START_FREQ; freq <= END_FREQ; freq += STEP) {
    // set frequency
    LoRa.setFrequency(freq);

    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      // received a packet
      Serial.print("Received packet at frequency ");
      Serial.print(freq);
      Serial.print(" : ");

      // read packet
      while (LoRa.available()) {
        Serial.print((char)LoRa.read());
      }

      //print distance in dBm
      int rssi = LoRa.packetRssi();
      double distance_dbm = pathLoss(rssi, TRANSMITTED_POWER, PATH_LOSS_EXPONENT);
      Serial.print(" with distance in dBm ");
      Serial.println(distance_dbm);
      //print distance in meters
      double distance_m = dbmToMeter(distance_dbm);
      Serial.print(" with distance in meters ");
      Serial.println(distance_m);
    }
  }
}

double pathLoss(int rssi, double txPower, double pathLossExponent) {
  return txPower - rssi;
}

double dbmToMeter(double dbm) {
  return pow(10, (27.55 - (20 * log10(START_FREQ)) + abs(dbm)) / 20);
}
