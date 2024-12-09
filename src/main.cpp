#include <Arduino.h>
#include "WifiConfig.h"
#include "RfidConfig.h"
#include "OledConfig.h"
#include "FirebaseConfig.h"
#include "BuzzerConfig.h"
#include <ezButton.h>

ezButton button(0);
unsigned long delayMillis = 0;
unsigned long lastControlUpdate = 0;
const unsigned long controlUpdateInterval = 30000;
MyOLED myOled;
Buzzer buzzer(2);
void setup()
{
    Serial.begin(115200);
    button.setDebounceTime(50); // set debounce time to 50 milliseconds
    button.setCountMode(COUNT_FALLING);
    // Inisialisasi OLED
    if (!myOled.begin())
    {
        Serial.println(F("Gagal menginisialisasi OLED!"));
        while (true)
            ; // Hentikan program jika gagal
    }

    while (WiFi.status() != WL_CONNECTED)
    {
        myOled.displayText("Hubungkan Wifi!", 2);
        connectToWiFi();
        if (WiFi.status() == WL_CONNECTED)
        {
            break;
        }
    }
    myOled.displayText("Terkoneksi", 2);
    myOled.displayText("Wifi!", 2, 42, 1);
    setupFire();
    updateControlValues();
    initRfid();
    initTime();
}

void Absensi()
{
    String tagData = readTag();
    if (tagData != "")
    {
        Serial.println("RFID Terbaca!");
        myOled.displayText("RFID Terbaca!", 2);
        buzzer.beep(1, 70);
        if (isUidExistsAndGetData(tagData))
        {
            String strownerName = fbdo.stringData();
            char ownerName[50];
            strownerName.toCharArray(ownerName, sizeof(ownerName));
            if (!isUidExists(tagData))
            {
                if (sendDatatoFirebase(tagData))
                {
                    Serial.println("Tag dikirim: " + tagData);
                    buzzer.beep(2, 70);
                    Serial.println("Sudah Presensi");
                    myOled.displayText(ownerName, 2, 0);
                    myOled.displayText("Hadir!", 2, 21, 1);
                    delay(2000);
                }
                else
                    Serial.println("Gagal mengirim tag: " + tagData);
            }
            else
            {
                Serial.println("Sudah Pernah absensi");
                buzzer.beep(2, 70);
                myOled.displayText(ownerName, 2, 0);
                myOled.displayText("Sudah", 2, 21, 1);
                myOled.displayText("Presensi!", 2, 42, 1);
                delay(2000);
            }
        }
        else
        {
            buzzer.beep(3, 70);
            Serial.println("UID tidak terdaftar");
            char UID[50];
            tagData.toCharArray(UID, sizeof(UID));
            myOled.displayText(UID, 2);
            myOled.displayText("Tidak", 2, 21, 1);
            myOled.displayText("Terdaftar!", 2, 42, 1);
            delay(2000);
        }
    }
    else // menunggu kartu ditap
    {
        Serial.println("Menunggu Kartu DiTap");
        myOled.displayText("Tap Kartu Anda!", 2);
    }
}

void Registrasi()
{
    String tagData = readTag();
    if (tagData != "")
    {
        if (sendRegisterData(tagData))
        {
            Serial.println("Tag dikirim: " + tagData);
            Serial.println("Registrasi Berhasil");
            myOled.displayText("Registrasi", 2, 0);
            myOled.displayText("Berhasil!", 2, 21, 1);
            delay(2000);
        }
        else
            Serial.println("Gagal mengirim tag: " + tagData);
    }
    else // menunggu kartu ditap
    {
        Serial.println("Menunggu Kartu DiTap");
        myOled.displayText("Tap Kartu Anda!", 2);
    }
}

void loop()
{
    button.loop();
    unsigned long switchValue = button.getCount();
    if (WiFi.status() == WL_CONNECTED) // wifi connected
    {
        if (millis() - delayMillis > 500) // read after 500ms
        {
            Serial.println(switchValue);
            delayMillis = millis();
            if (Firebase.ready()) // firebase ready
            {
                updateControlValues();
                if (switchValue <= 1)
                {
                    if (switchValue == 0) // Mode absensi
                    {
                        Absensi();
                        Serial.println("Mode: Absensi");
                        myOled.displayText("Mode: Absensi", 2);
                    }
                    else if (switchValue == 1) // Mode registrasi
                    {
                        Registrasi();
                        Serial.println("Mode: Registrasi");
                        myOled.displayText("Mode: Registrasi", 2);
                    }
                }
                else // reset count
                {
                    button.resetCount(); // Reset jika count lebih dari 2
                }
            }
            else // reconnect firebase
            {
                Serial.println("Tidak Terkoneksi Firebase!");
                reconnectFirebase();
            }
        }
    }
    else // reconnect wifi
    {
        Serial.println("Tidak Terkoneksi Wifi!");
        myOled.displayText("Tidak", 2, 0);
        myOled.displayText("Terkoneksi", 2, 21, 1);
        myOled.displayText("Wifi!", 2, 42, 1);
        reconnectWiFi();
    }
}
