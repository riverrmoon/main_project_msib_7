#ifndef TIME_CONFIG_H
#define TIME_CONFIG_H

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>

WiFiUDP ntpUDP;
const long utcOffsetSeconds = 25200; // UTC+7 (WIB)
NTPClient timeClient(ntpUDP, "id.pool.ntp.org", utcOffsetSeconds);

// Nama hari dan bulan
const String weekDays[7] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
const String months[12] = {"Januari", "Februari", "Maret", "April", "Mei", "Juni", "Juli", "Agustus", "September", "Oktober", "November", "Desember"};

// Inisialisasi NTP Client
void initTime()
{
    timeClient.begin();
    timeClient.update();
}

// Mendapatkan waktu dalam format HH:MM:SS
String getTime()
{
    timeClient.update();
    return timeClient.getFormattedTime();
}

// Mendapatkan hari dalam seminggu
String getWeekDay()
{
    timeClient.update();
    return weekDays[timeClient.getDay()];
}

// Mendapatkan tanggal saat ini dalam format YYYY-MM-DD
String getDate()
{
    timeClient.update();
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);

    int year = ptm->tm_year + 1900;
    int month = ptm->tm_mon + 1;
    int day = ptm->tm_mday;

    return String(year) + "-" + String(month) + "-" + String(day);
}

// Mendapatkan nama bulan
String getMonthName()
{
    timeClient.update();
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);

    int month = ptm->tm_mon; // Bulan dalam `struct tm` mulai dari 0
    return months[month];
}

// Mendapatkan tahun saat ini
int getYear()
{
    timeClient.update();
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);

    return ptm->tm_year + 1900;
}

// Mendapatkan bulan saat ini
int getMonth()
{
    timeClient.update();
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);

    return ptm->tm_mon + 1; // Bulan dalam `struct tm` mulai dari 0
}

// Mendapatkan hari dalam sebulan
int getDay()
{
    timeClient.update();
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);

    return ptm->tm_mday;
}

#endif // TIME_CONFIG_H
