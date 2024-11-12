#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <ESP8266WiFi.h>

// Konfigurasi SSID dan Password Wi-Fi
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// Fungsi untuk menghubungkan perangkat ke Wi-Fi
void setupWiFi() {
    Serial.print("Menghubungkan ke WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi terhubung!");
}

#endif
