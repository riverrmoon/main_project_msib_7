#ifndef TIME_CONFIG_H
#define TIME_CONFIG_H

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>

WiFiUDP ntpUDP;
const long utcOffsetSeconds = 0; // UTC+7 (WIB) 25200 = 7 jam
NTPClient timeClient(ntpUDP, "id.pool.ntp.org", utcOffsetSeconds);
const String weekDays[7] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
const String months[12] = {"Januari", "Februari", "Maret", "April", "Mei", "Juni", "Juli", "Agustus", "September", "Oktober", "November", "Desember"};

void initTime()
{
    timeClient.begin();
}

int getHour()
{
    timeClient.update();
    return timeClient.getHours();
}

int getMinute()
{
    timeClient.update();
    return timeClient.getMinutes();
}

String getTime()
{
    timeClient.update();
    return timeClient.getFormattedTime();
}

String getWeekDay()
{
    timeClient.update();
    return weekDays[timeClient.getDay()];
}

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

String getMonthName()
{
    timeClient.update();
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);

    int month = ptm->tm_mon; // Bulan dalam `struct tm` mulai dari 0
    return months[month];
}

int getYear()
{
    timeClient.update();
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);

    return ptm->tm_year + 1900;
}

int getMonth()
{
    timeClient.update();
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);

    return ptm->tm_mon + 1; // Bulan dalam `struct tm` mulai dari 0
}

int getDay()
{
    timeClient.update();
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);

    return ptm->tm_mday;
}

#endif
