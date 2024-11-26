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

const int menitToleransi = 15;
const int jamLimitBatasBawah = 6;
const int jamAbsenMasuk = 8;
const int jamLimitAbsenMasuk = 10;
const int jamLimitBatasAtas = 15;
const int jamLimitAbsenKeluar = 19;

// function for validation time
bool valTepatWaktu() // efektif dari jam >=6-<=8 <=15 menit
{
  if (getHour() >= jamLimitBatasBawah && getHour() <= jamAbsenMasuk)
  {
    if (getMinute() <= menitToleransi)
      return true;
    else
      return false;
  }
  else
    return false;
}
bool valSedikitTerlambat() // efektif dari jam >=6-<=8 >15 menit
{
  if (getHour() >= jamLimitBatasBawah && getHour() <= jamAbsenMasuk)
  {
    if (getMinute() > menitToleransi)
      return true;
    else
      return false;
  }
  else
    return false;
}
bool valTerlambat() // efektif dari jam >=8-<=10
{
  if (getHour() >= jamLimitBatasBawah && getHour() <= jamLimitAbsenMasuk)
    return true;
  else
    return false;
}
bool valAbsenKeluar() // efektif dari jam >=15 - >=19
{
  if (getHour() >= jamLimitBatasAtas && getHour() <= jamLimitAbsenKeluar)
    return true;
  else
    return false;
}
bool valCantSend() // efektif dari jam >10 - <15
{
  if (getHour() < jamLimitBatasBawah || (getHour() > jamLimitAbsenMasuk && getHour() < jamLimitBatasAtas) || getHour() > jamLimitAbsenKeluar)
    return true;
  else
    return false;
}

// function for status absensi
String getStatusAbsensi()
{
  if (getHour() >= jamLimitBatasBawah && getHour() <= jamLimitAbsenMasuk)
  {
    return "absensiMasuk";
  }
  else
  {
    return "absensiKeluar";
  }
}
String getStatus()
{
  if (valTepatWaktu())
  {
    return "Tepat Waktu";
  }
  else if (valSedikitTerlambat())
  {
    return "Sedikit Terlambat";
  }
  else if (valTerlambat())
  {
    return "Terlambat";
  }
  else
  {
    return "error";
  }
}

// function for path Absensi Masuk
String path(const String &tagData)
{
  String tagAbsensi = getStatusAbsensi();
  return "MSIB7/dataBankSampahPerwira/dataAbsensi/" + getDate() + "/" + tagAbsensi + "/" + tagData;
}
String pathUID(const String &tagData)
{
  return path(tagData) + "/uid";
}
String pathWaktu(const String &tagData)
{
  return path(tagData) + "/waktu";
}
String pathStatus(const String &tagData)
{
  return path(tagData) + "/status";
}

bool sendPath(const String &tagData)
{
  if (getHour() >= jamLimitBatasBawah && getHour() <= jamLimitAbsenMasuk)
  {
    Firebase.RTDB.setString(&fbdo, pathUID(tagData), String(tagData)) && Firebase.RTDB.setString(&fbdo, pathWaktu(tagData), getTime()) && Firebase.RTDB.setString(&fbdo, pathStatus(tagData), (getStatus()));
    return true;
  }
  else
  {
    Firebase.RTDB.setString(&fbdo, pathUID(tagData), String(tagData)) && Firebase.RTDB.setString(&fbdo, pathWaktu(tagData), getTime());
    return false;
  }
}

// function for setup firebase
bool setupFire()
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
    return true;
  }
  else
  {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
    return false;
  }
}

// function for reconnect firebase
//  void reconnectFirebase()
//  {
//    if (!Firebase.signUp(&config, &auth, "", ""))
//    {
//      Serial.println("Firebase Error: " + fbdo.errorReason());
//    }
//    else
//      Firebase.begin(&config, &auth);
//  }

// function for send data
void sendDataTepatWaktu(const String &tagData)
{
  if (sendPath(tagData))
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
void sendDataSedikitTerlambat(const String &tagData)
{
  if (sendPath(tagData))
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
void sendDataTerlambat(const String &tagData)
{
  if (sendPath(tagData))
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
void sendDataAbsenKeluar(const String &tagData)
{
  if (sendPath(tagData))
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

// function for validation & send data
bool sendDatatoFirebase(const String &tagData)
{
  if (valTepatWaktu())
  {
    sendDataTepatWaktu(tagData);
    return true;
  }
  else if (valSedikitTerlambat())
  {
    sendDataSedikitTerlambat(tagData);
    return true;
  }
  else if (valTerlambat())
  {
    sendDataTerlambat(tagData);
    return true;
  }
  else if (valCantSend())
  {
    Serial.println("Error : belum ready");
    Serial.println(getHour());
    return false;
  }
  else if (valAbsenKeluar())
  {
    sendDataAbsenKeluar(tagData);
    return true;
  }
  else
  {
    Serial.println("Error : " + fbdo.errorReason());
    Serial.println(getHour());
    return false;
  }
}

bool isUidExistsAndGetData(String uid)
{
  String path = "MSIB7/dataBankSampahPerwira/dataKaryawanAbsensi/" + uid + "/Nama";
  if (Firebase.RTDB.getString(&fbdo, path))
  {
    if (fbdo.stringData() != "")
    {
      // Jika data ditemukan, simpan nama pemilik ke variabel referensi
      String ownerName = fbdo.stringData();
      Serial.println("UID ditemukan: " + uid + ", Nama Pemilik: " + ownerName);
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    Serial.println("Gagal membaca UID dari Firebase: " + String(fbdo.errorReason().c_str()));
    return false;
  }
}

bool isUidExists(const String &tagData)
{
  String pathUID = path(tagData);
  if (Firebase.RTDB.getString(&fbdo, pathUID))
  {
    if (fbdo.stringData() == "")
    {
      return false;
    }
    else
    {
      return true;
    }
  }
  return false;
}

#endif