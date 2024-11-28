#include <Arduino.h>
#include "WifiConfig.h"
#include "MqttConfig.h"
#include "RfidConfig.h"
#include "OledConfig.h"
#include "FirebaseConfig.h"
#include "BuzzerConfig.h"


bool signUp = false;
unsigned long delayMillis;
// Membuat instance MyOLED
MyOLED myOled;
Buzzer buzzer(2);

void setup()
{   
    Serial.begin(115200);
    // Inisialisasi OLED
    if (!myOled.begin()) 
    {
        Serial.println(F("Gagal menginisialisasi OLED!"));
        while (true); // Hentikan program jika gagal
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
    Serial.println("Connected to WiFi!");
    myOled.displayText("Terkoneksi", 2);
    myOled.displayText("Wifi!", 2, 42, 1);
    do
    {
        if (millis() - delayMillis > 500 || delayMillis == 0)
        {
            delayMillis = millis();
            if (setupFire())
            {
                break;
            }
            Serial.println("Firebase setup failed.");
            myOled.displayText("Gagal", 2, 0);
            myOled.displayText("Setup", 2, 21, 1);
            myOled.displayText("Firebase!", 2, 42, 1);
        }
    } while (!Firebase.ready());
    signUp = true;
    initRfid();
    initTime();
}

void loop()
{   
    if (WiFi.status() == WL_CONNECTED)
    {
        if (millis() - delayMillis > 500)
        {
            delayMillis = millis();
            if (Firebase.ready() && signUp)
            {
                String tagData = readTag();
                if (tagData != "")
                {
                    Serial.println("RFID Terbaca!");
                    myOled.displayText("RFID      Terbaca!", 2);
                    buzzer.beep(1, 70);
                    if (isUidExistsAndGetData(tagData))
                    {   
                        String strownerName = fbdo.stringData();
                        char ownerName[50];
                        strownerName.toCharArray(ownerName, sizeof(ownerName));
                        if (!isUidExists(tagData))
                        {
                            sendDatatoFirebase(tagData) ? Serial.println("Tag dikirim: " + tagData) : Serial.println("Gagal mengirim tag: " + tagData);
                            buzzer.beep(2, 70);
                            Serial.println("Sudah Presensi");
                            myOled.displayText(ownerName, 2, 0);
                            myOled.displayText("Hadir!", 2, 21, 1);
                            delay(2000);
                        }
                        else
                        {
                            Serial.println("Sudah Pernah absensi");
                            buzzer.beep(2, 70);
                            Serial.println("Sudah Presensi");
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
                        delay(3000);
                    }
                }
                else
                {
                    Serial.println("Menunggu Kartu DiTap");
                    myOled.displayText("Tap Kartu Anda!", 2);
                }
            }
            else
            {
                reconnectFirebase();
            }
        }
    }
    else
    {   
        Serial.println("Tidak Terkoneksi Wifi!");
        myOled.displayText("Tidak", 2, 0);
        myOled.displayText("Terkoneksi", 2, 21, 1);
        myOled.displayText("Wifi!", 2, 42, 1);
        reconnectWiFi();
    }
}
