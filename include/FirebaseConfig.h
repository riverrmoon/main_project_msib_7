#ifndef FIREBASE_CONFIG_H
#define FIREBASE_CONFIG_H

#include <Firebase_ESP_Client.h>

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include "TimeConfig.h"
#define API_KEY "AIzaSyDmnYvJv7QkXUZc-T3wxQKnjTUKpy8QnJM"
#define DATABASE_URL "https://msib-7-ta-ldre-default-rtdb.asia-southeast1.firebasedatabase.app/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
void setupFire()
{
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;

  config.database_url = DATABASE_URL;
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
  Serial.print("Sign up new user... ");

  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("ok");
    config.token_status_callback = tokenStatusCallback;
    Firebase.begin(&config, &auth);
  }
  else
  {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
}

// bool setupFirebase()
// {
//   config.api_key = API_KEY;
//   config.database_url = DATABASE_URL;
//   Firebase.reconnectNetwork(true);
//   fbdo.setBSSLBufferSize(4096, 1024);
//   if (Firebase.signUp(&config, &auth, "", ""))
//   {
//     Serial.println("Firebase setup completed.");
//     config.token_status_callback = tokenStatusCallback;
//     Firebase.begin(&config, &auth);
//     return true;
//   }
//   else
//   {
//     Serial.printf("%s\n", config.signer.signupError.message.c_str());
//     return false;
//   }
// }

// void reconnectFirebase()
// {
//   if (!Firebase.signUp(&config, &auth, "", ""))
//   {
//     Serial.println("Firebase Error: " + fbdo.errorReason());
//   }
//   else
//     Firebase.begin(&config, &auth);
// }

void sendDatatoFirebase(const String &tagData)
{
  String pathUID = "MSIB7/dataBankSampahPerwira/dataAbsensi/" + tagData + "/uid";
  String pathWaktu = "MSIB7/dataBankSampahPerwira/dataAbsensi/" + tagData + "/waktu";
  if (Firebase.RTDB.setString(&fbdo, pathUID, String(tagData)) && Firebase.RTDB.setString(&fbdo, pathWaktu, "waktu"))
  {
    Serial.println("Data Sent to Firebase!");
    Serial.println("Path: " + fbdo.dataPath());
  }
  else
  {
    Serial.println("Failed to Send Data.");
    Serial.print("Reason: ");
    Serial.println(fbdo.errorReason());
  }
}

#endif