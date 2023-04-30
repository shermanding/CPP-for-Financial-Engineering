#include "Date.h"

double operator-(const datetime_t& d1, const datetime_t& d2)
{
    int yearDiff = d1.year - d2.year;
    int monthDiff = d1.month - d2.month;
    int dayDiff = d1.day - d2.day;
    int hourDiff = d1.hour - d2.hour;
    int minDiff = d1.min - d2.min;
    int secDiff = d1.sec - d2.sec;
    return yearDiff + monthDiff / 12.0 + dayDiff / 365.0 + hourDiff / 8760.0 + minDiff / 525600.0 + secDiff / 31536000.0;
}

bool operator<(const datetime_t& d1, const datetime_t& d2)
{
    return d2 - d2 > 0;
}

std::ostream& operator<<(std::ostream& os, const datetime_t& d)
{
  std::string month;
  std::string day;
  std::string hour;
  std::string min;
  std::string sec;
  std::string ms;
  if (d.month<10){
    month = "0"+std::to_string(d.month);
  } else {
    month = std::to_string(d.month);
  }
  if (d.day<10){
    day = "0"+std::to_string(d.day);
  } else {
    day = std::to_string(d.day);
  }
  if (d.hour<10){
    hour = "0"+std::to_string(d.hour);
  } else {
    hour = std::to_string(d.hour);
  }
  if (d.min<10){
    min = "0"+std::to_string(d.min);
  } else {
    min = std::to_string(d.min);
  }
  if (d.sec<10){
    sec = "0"+std::to_string(d.sec);
  } else {
    sec = std::to_string(d.sec);
  }
  if (d.ms<10){
    ms = "00"+std::to_string(d.ms);
  } else if (d.ms<100) {
    ms = "0"+std::to_string(d.ms);
  } else {
    ms = std::to_string(d.ms);
  }
  os << d.year << "-" << month << "-" << day << "-"<<"T"<<hour<<":"<<min<<":"<<sec<<"."<<ms<<"Z";
  return os;
}

std::istream& operator>>(std::istream& is, datetime_t& d)
{
  is >> d.year >> d.month >> d.day;
  return is;
}
