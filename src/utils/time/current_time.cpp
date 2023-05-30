#include "current_time.h"
#include <ctime>

WString WDate::toString() const
{
    return WString("%1 %2 %3")
        .arg(this->_year)
        .arg(this->_mount)
        .arg(this->_day);
}

WDate::WDate(int year, int mount, int day)
    : _year((char) year)
    , _mount((char) mount)
    , _day((char) day)
{
    W_ASSERT(year < CHAR_MAX);
    W_ASSERT(mount >= 1 and mount <= 12);
    W_ASSERT(day >= 1 && day <= 31);
}

WDate::WDate()
    : _year(0)
    , _mount(0)
    , _day(0)
{

}

WDate WDate::now()
{
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    return {
        localTime->tm_year + 1900,
        localTime->tm_mon + 1,
        localTime->tm_mday,
    };
}

WTime::WTime(int h, int m, int s)
    : _hour(h)
    , _minute(m)
    , _second(s)
{

}

WString WTime::toString() const
{
    return WString("%1 %2 %3")
        .arg(_hour)
        .arg(_minute)
        .arg(_second);
}

WTime WTime::now()
{
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    return {
            localTime->tm_hour,
            localTime->tm_min,
            localTime->tm_sec
    };
}
