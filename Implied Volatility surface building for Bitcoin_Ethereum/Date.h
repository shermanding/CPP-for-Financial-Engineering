#ifndef QF633_DATE_H
#define QF633_DATE_H

#include <iostream>
#include <ctime>
#include <map>

class datetime_t { // hold date[time] (interpreted as UTC)
public:
    int year;
    int month;
    int day;
    int hour = 0;
    int min = 0;
    int sec = 0;
    int ms = 0;
    datetime_t(int y, int m, int d, int hour_ = 0, int min_ = 0, int sec_ = 0) : year(y), month(m), day(d), hour(hour_), min(min_), sec(sec_){};
    datetime_t(){}
    datetime_t(uint64_t unix_epoch_sec) {
        const time_t second = unix_epoch_sec/1000;
        auto tm = std::gmtime(&second);
        year = tm->tm_year+1900;
        month = tm->tm_mon+1;
        day = tm->tm_mday;
        hour = tm->tm_hour;
        min = tm->tm_min;
        sec = tm->tm_sec;
        ms = unix_epoch_sec%1000;
    }
};
double operator-(const datetime_t& d1, const datetime_t& d2);
bool operator<(const datetime_t& d1, const datetime_t& d2);
std::ostream& operator<<(std::ostream& os, const datetime_t& date);
std::istream& operator>>(std::istream& is, datetime_t& date);

#endif