#include <Arduino.h>
#include "WifiConfig.h"
#include "MqttConfig.h"
#include "RfidConfig.h"
#include "OledConfig.h"
#include "FirebaseConfig.h"

// const char* mqttServer = "broker.hivemq.com";
// const int mqttPort = 1883;
// const char* mqttTopic = "rfid/tag";

void setup()
{
    Serial.begin(115200);
    // mdnsInit();
    WiFi.status() != WL_CONNECTED ? connectToWiFi() : Serial.println("Connected to WiFi!");
    // setupMqtt(mqttServer, mqttPort);
    // connectToBroker();
    initRfid(); // Inisialisasi pembaca RFID
}

void loop()
{
    // Memastikan koneksi ke broker MQTT tetap tersambung
    // if (!mqttClient.connected()) {
    //     connectToBroker();
    // }
    // mqttClient.loop();

    // // Membaca data dari tag RFID
    // String tagData = readTag();
    // if (tagData != "") {  // Jika ada tag yang terbaca
    //     sendMqttMessage(mqttTopic, tagData); // Mengirim data melalui MQTT
    //     Serial.println("Tag dikirim: " + tagData);
    // }

    // delay(1000); // Delay untuk pembacaan berikutnya
    String tagData = readTag();
    if (tagData != "")
    {
    }
}
