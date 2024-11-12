#ifndef RFID_CONFIG_H
#define RFID_CONFIG_H

#include <SPI.h>
#include <RFID.h>

// Konfigurasi pin RFID
#define SS_PIN 5
#define RST_PIN 22

RFID rfid(SS_PIN, RST_PIN); // Objek RFID

// Fungsi untuk inisialisasi modul RFID
void initRfid() {
    SPI.begin();
    rfid.init();
    Serial.println("RFID Reader diinisialisasi!");
}

// Fungsi untuk membaca tag RFID
String readTag() {
    String tagData = "";
    if (rfid.isCard()) {
        if (rfid.readCardSerial()) {
            // Mengonversi data ID tag ke format String
            tagData += String(rfid.serNum[0], HEX);
            tagData += String(rfid.serNum[1], HEX);
            tagData += String(rfid.serNum[2], HEX);
            tagData += String(rfid.serNum[3], HEX);

            // Menampilkan hasil di Serial Monitor
            Serial.println(" ");
            Serial.println("Card found");
            Serial.println("Cardnumber:");
            Serial.println(tagData);
        }
    }
    rfid.halt();  // Menghentikan pembacaan kartu

    return tagData;  // Mengembalikan ID tag sebagai String
}

#endif
