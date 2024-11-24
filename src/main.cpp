#include <Arduino.h>
#include "WifiConfig.h"
#include "MqttConfig.h"
#include "RfidConfig.h"
#include "OledConfig.h"
#include "FirebaseConfig.h"

bool signUp = false;
unsigned long delayMillis;
void setup()
{
    Serial.begin(115200);
    while (WiFi.status() != WL_CONNECTED)
    {
        connectToWiFi();
        if (WiFi.status() == WL_CONNECTED)
        {
            break;
        }
    }
    Serial.println("Connected to WiFi!");
    do
    {
        if (millis() - delayMillis > 500 || delayMillis == 0)
        {
            delayMillis = millis();
            setupFire();
            Serial.println("Firebase setup failed.");
        }
    } while (!Firebase.ready());
    signUp = true;
    initRfid();
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        if (millis() - delayMillis > 1000)
        {
            delayMillis = millis();
            if (Firebase.ready() && signUp)
            {
                String tagData = readTag();
                if (tagData != "")
                {
                    sendDatatoFirebase(tagData);
                    Serial.println("Tag dikirim: " + tagData);
                }
                else
                {
                    Serial.println("Menunggu Kartu DiTap");
                }
            }
            else
            {
                // setupFirebase();
            }
        }
    }
    else
    {
        reconnectWiFi();
    }
}
