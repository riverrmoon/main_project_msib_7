#ifndef MQTT_CONFIG_H
#define MQTT_CONFIG_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Objek WiFi dan MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Fungsi untuk mengatur MQTT server
void setupMqtt(const char* server, int port) {
    mqttClient.setServer(server, port);
}

// Fungsi untuk menghubungkan ke broker MQTT
void connectToBroker() {
    while (!mqttClient.connected()) {
        Serial.print("Menghubungkan ke MQTT Broker...");
        if (mqttClient.connect("RFIDClient")) {
            Serial.println("Terhubung!");
        } else {
            Serial.print("Gagal, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" mencoba lagi dalam 5 detik...");
            delay(5000);
        }
    }
}

// Fungsi untuk mengirimkan pesan MQTT
void sendMqttMessage(const char* topic, const String& payload) {
    mqttClient.publish(topic, payload.c_str());
}

#endif
