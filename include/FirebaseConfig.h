#ifndef FIREBASE_CONFIG_H
#define FIREBASE_CONFIG_H

#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include "TimeConfig.h"
#define API_KEY "AIzaSyDmnYvJv7QkXUZc-T3wxQKnjTUKpy8QnJM"
#define DATABASE_URL "https://msib-7-ta-ldre-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define USER_EMAIL "ibadcek@gmail.com"
#define USER_PASSWORD "ibadcek@gmail.com"
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

int menitToleransi = 15;
int jamLimitBatasBawah = 6;
int jamAbsenMasuk = 8;
int jamLimitAbsenMasuk = 10;
int jamAbsenKeluar = 15;
int jamLimitAbsenKeluar = 19;

String getPathControl()
{
  String path = "MSIB7/dataBankSampahPerwira/controlInput/inputWaktu/";
  return path;
}
int getMinuteToleransi()
{
  String path = getPathControl() + "toleranMenit";
  if (Firebase.RTDB.getInt(&fbdo, path))
  {
    if (fbdo.stringData() != "")
    {
      int menit = fbdo.intData();
      return menit;
    }
    else
      return 15;
  }
  else
    return 15;
}
int getJamLimitBatasBawah()
{
  String path = getPathControl() + "jamLimitBatasBawah";
  if (Firebase.RTDB.getInt(&fbdo, path))
  {
    if (fbdo.stringData() != "")
    {
      int jamLimitBatasBawah = fbdo.intData();
      return jamLimitBatasBawah;
    }
    else
      return 6;
  }
  else
    return 6;
}
int getJamAbsenMasuk()
{
  String path = getPathControl() + "jamAbsenMasuk";
  if (Firebase.RTDB.getInt(&fbdo, path))
  {
    if (fbdo.stringData() != "")
    {
      int jamAbsenMasuk = fbdo.intData();
      return jamAbsenMasuk;
    }
    else
      return 8;
  }
  else
    return 8;
}
int getJamLimitAbsenMasuk()
{
  String path = getPathControl() + "jamLimitAbsenMasuk";
  if (Firebase.RTDB.getInt(&fbdo, path))
  {
    if (fbdo.stringData() != "")
    {
      int jamLimitAbsenMasuk = fbdo.intData();
      return jamLimitAbsenMasuk;
    }
    else
      return 15;
  }
  else
    return 15;
}
int getJamAbsenKeluar()
{
  String path = getPathControl() + "jamAbsenKeluar";
  if (Firebase.RTDB.getInt(&fbdo, path))
  {
    if (fbdo.stringData() != "")
    {
      int jamAbsenKeluar = fbdo.intData();
      return jamAbsenKeluar;
    }
    else
      return 15;
  }
  else
    return 15;
}
int getJamLimitAbsenKeluar()
{
  String path = getPathControl() + "limitJamAbsenKeluar";
  if (Firebase.RTDB.getInt(&fbdo, path))
  {
    if (fbdo.stringData() != "")
    {
      int limitJamAbsenKeluar = fbdo.intData();
      return limitJamAbsenKeluar;
    }
    else
      return 19;
  }
  else
    return 19;
}

void updateControlValues()
{
  menitToleransi = getMinuteToleransi();
  jamLimitBatasBawah = getJamLimitBatasBawah();
  jamAbsenMasuk = getJamAbsenMasuk();
  jamLimitAbsenMasuk = getJamLimitAbsenMasuk();
  jamAbsenKeluar = getJamAbsenKeluar();
  jamLimitAbsenKeluar = getJamLimitAbsenKeluar();

  Serial.println("Kontrol Firebase diperbarui:");
  Serial.println("menitToleransi: " + String(menitToleransi));
  Serial.println("jamLimitBatasBawah: " + String(jamLimitBatasBawah));
  Serial.println("jamAbsenMasuk: " + String(jamAbsenMasuk));
  Serial.println("jamLimitAbsenMasuk: " + String(jamLimitAbsenMasuk));
  Serial.println("jamAbsenKeluar: " + String(jamAbsenKeluar));
  Serial.println("jamLimitAbsenKeluar: " + String(jamLimitAbsenKeluar));
}

void firebaseStreamCallback(FirebaseStream data)
{
  Serial.println("Data kontrol diperbarui dari Firebase.");
  updateControlValues();
}

void firebaseStreamTimeoutCallback(bool timeout)
{
  if (timeout)
  {
    Serial.println("Stream timeout, mencoba ulang...");
  }
}

// function for validation time
bool valTepatWaktu() // Efektif dari jam >= 6 sampai <= 8:15
{
  int currentHour = getHour();
  int currentMinute = getMinute();

  if (currentHour >= jamLimitBatasBawah && currentHour < jamAbsenMasuk)
  {
    return true;
  }
  else if (currentHour == jamAbsenMasuk && currentMinute <= menitToleransi)
  {
    return true;
  }
  return false;
}
bool valSedikitTerlambat() // efektif dari jam 8 >15 menit
{
  if (getHour() == jamAbsenMasuk)
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
  if (getHour() > jamAbsenMasuk && getHour() <= jamLimitAbsenMasuk)
    return true;
  else
    return false;
}
bool valAbsenKeluar() // efektif dari jam >=15 - >=19
{
  if (getHour() >= jamAbsenKeluar && getHour() <= jamLimitAbsenKeluar)
    return true;
  else
    return false;
}
bool valCantSend() // efektif dari jam >10 - <15
{
  if (getHour() < jamLimitBatasBawah || (getHour() > jamLimitAbsenMasuk && getHour() < jamAbsenKeluar) || getHour() > jamLimitAbsenKeluar)
    return true;
  else
    return false;
}
// function for validation absen masuk dan keluar
bool sendAbsenMasuk() // jam masuk >= 6 - <= 10
{
  if (getHour() >= jamLimitBatasBawah && getHour() <= jamLimitAbsenMasuk)
  {
    return true;
  }
  else
    return false;
}
// function for status absensi
String getStatusAbsensi()
{
  if (sendAbsenMasuk())
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
// function for path Absensi
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
// function for register data
String pathRegisterUID(const String &tagData)
{
  return "MSIB7/dataBankSampahPerwira/dataRegister/" + tagData + "/uid";
}
String pathRegisterTime(const String &tagData)
{
  return "MSIB7/dataBankSampahPerwira/dataRegister/" + tagData + "/waktu";
}
// function for send to firebase
bool sendPath(const String &tagData)
{
  if (sendAbsenMasuk())
  {
    if (Firebase.RTDB.setString(&fbdo, pathUID(tagData), String(tagData)) && Firebase.RTDB.setString(&fbdo, pathWaktu(tagData), getTime()) && Firebase.RTDB.setString(&fbdo, pathStatus(tagData), getStatus()))
      return true;
    else
      return false;
  }
  else if (valAbsenKeluar())
  {
    if (Firebase.RTDB.setString(&fbdo, pathUID(tagData), String(tagData)) && Firebase.RTDB.setString(&fbdo, pathWaktu(tagData), getTime()))
      return true;
    else
      return false;
  }
  else
    return false;
}
// function for setup firebase
void setupFire()
{
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
}
// function for reconnect firebase
void reconnectFirebase()
{
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
}
// function for validation & send data
bool sendDatatoFirebase(const String &tagData)
{
  if (sendPath(tagData))
  {
    return true;
  }
  else if (valCantSend())
  {
    Serial.println("Error : tidak diperbolehkan absen");
    Serial.println(getHour());
    return false;
  }
  else
  {
    Serial.println("Error : " + fbdo.errorReason());
    return false;
  }
}
// function for validation and get data uid
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
// function for validation if uid exist in same day
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
// function for data registrasion
bool sendRegisterData(const String &tagData)
{
  Serial.print(menitToleransi + " " + jamLimitBatasBawah);
  Serial.print(jamAbsenMasuk + " " + jamLimitAbsenMasuk);
  Serial.println(jamAbsenKeluar + " " + jamLimitAbsenKeluar);
  if (Firebase.RTDB.setString(&fbdo, pathRegisterUID(tagData), String(tagData)) && Firebase.RTDB.setString(&fbdo, pathRegisterTime(tagData), getTime()))
  {
    return true;
  }
  return false;
}
#endif