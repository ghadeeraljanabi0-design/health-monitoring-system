
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define DHTPIN 4
#define DHTTYPE DHT22

#define ONE_WIRE_BUS 5   // DS18B20 data pin

PulseOximeter pox;
DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

uint32_t lastReport = 0;

void onBeatDetected() {
  Serial.println("❤️ Beat Detected!");
}

void setup() {
  Serial.begin(115200);

  // MAX30100
  Serial.println("Initializing MAX30100...");
  if (!pox.begin()) {
    Serial.println("Failed to initialize MAX30100!");
    while (1);
  }
  pox.setOnBeatDetectedCallback(onBeatDetected);

  // DHT22
  dht.begin();

  // DS18B20
  sensors.begin();

  Serial.println("System Ready ✔️");
}

void loop() {
  // Update MAX30100
  pox.update();

  // Read every 1 second
  if (millis() - lastReport > 1000) {
    lastReport = millis();

    float bpm = pox.getHeartRate();
    float spo2 = pox.getSpO2();

    float h = dht.readHumidity();
    float t = dht.readTemperature();  // DHT22 temp

    sensors.requestTemperatures();
    float bodyTemp = sensors.getTempCByIndex(0); // DS18B20 temp

    Serial.println("====================================");
    Serial.print("❤️ Heart Rate: ");
    Serial.println(bpm);

    Serial.print("🩸 SpO2: ");
    Serial.println(spo2);

    Serial.print("🌡️ DHT22 Temp: ");
    Serial.println(t);

    Serial.print("💧 Humidity: ");
    Serial.println(h);

    Serial.print("🔥 DS18B20 Body Temp: ");
    Serial.println(bodyTemp);

    Serial.println("====================================");
  }
}


