#include "datetime.h"
#include <ctime>
DateTime::DateTime()
{
    time_t rawtime;
    struct tm * timeinfo;
    
    time (&rawtime);
    timeinfo = localtime(&rawtime);

    year = timeinfo->tm_year + 1900;
    month = timeinfo->tm_mon + 1;
    day = timeinfo->tm_mday;
    hour = timeinfo->tm_hour;
    min = timeinfo->tm_min;
    sec = timeinfo->tm_sec;
}

DateTime::DateTime(int hh, int mm, int ss, int year, int month, int day) : 
hour(hh),min(mm),sec(ss),year(year),month(month),day(day)
{}

bool DateTime::operator<(const DateTime& other) const
{
    // compare year
    if (this->year < other.year) {return true;}
    else if (this->year > other.year) {return false;}

    // compare month
    if (this->month < other.month) {return true;}
    else if (this->month > other.month) {return false;}

    // compare day
    if (this->day < other.day) {return true;}
    else if (this->day > other.day) {return false;}

    // compare hour
    if (this->hour < other.hour) {return true;}
    else if (this->hour > other.hour) {return false;}

    // compare min
    if (this->min < other.min) {return true;}
    else if (this->min > other.min) {return false;}

    // compare sec
    if (this->sec < other.sec) {return true;}
    else if (this->sec > other.sec) {return false;}

    // equal
    return false;
}

std::ostream& operator<<(std::ostream& os, const DateTime& other)
{
    os << other.year<< "-";

    if (other.month < 10)
    {
        os << "0" << other.month;
    }
    else
    {
        os << other.month;
    }
    os << "-";

    if (other.day < 10)
    {
        os << "0" << other.day;
    }
    else
    {
        os << other.day;
    }
    os << " ";

    if (other.hour < 10)
    {
        os << "0" << other.hour;
    }
    else
    {
        os << other.hour;
    }
    os << ":";

    if (other.min < 10)
    {
        os << "0" << other.min;
    }
    else
    {
        os << other.min;
    }
    os << ":";
    
    if (other.sec < 10)
    {
        os << "0" << other.sec;
    }
    else
    {
        os << other.sec;
    }
    return os;
}

std::istream& operator>>(std::istream& is, DateTime& dt)
{
    std::string dateStr, timeStr;
    is >> dateStr >> timeStr;
    if (is.fail())
    {
        time_t rawtime;
        struct tm * timeinfo;
        
        time (&rawtime);
        timeinfo = localtime(&rawtime);

        dt.year = timeinfo->tm_year + 1900;
        dt.month = timeinfo->tm_mon + 1;
        dt.day = timeinfo->tm_mday;
        dt.hour = timeinfo->tm_hour;
        dt.min = timeinfo->tm_min;
        dt.sec = timeinfo->tm_sec;
        return is;
    }

    // discarding the punctuation
    for(std::string::iterator it = dateStr.begin(); it != dateStr.end(); ++it)
    {
        if (*it == '-')
        {
            *it = ' ';
        }
    }
    for(std::string::iterator it = timeStr.begin(); it != timeStr.end(); ++it)
    {
        if (*it == ':')
        {
            *it = ' ';
        }
    }

    // extract year month day
    std::stringstream dateSS;
    dateSS << dateStr;
    dateSS >> dt.year >> dt.month >> dt.day;
    if (dateSS.fail())
    {
        time_t rawtime;
        struct tm * timeinfo;
        
        time (&rawtime);
        timeinfo = localtime(&rawtime);

        dt.year = timeinfo->tm_year + 1900;
        dt.month = timeinfo->tm_mon + 1;
        dt.day = timeinfo->tm_mday;
        dt.hour = timeinfo->tm_hour;
        dt.min = timeinfo->tm_min;
        dt.sec = timeinfo->tm_sec;
        return is;
    }

    // extract hour min sec
    std::stringstream timeSS;
    timeSS << timeStr;
    timeSS >> dt.hour >> dt.min >> dt.sec;
    if (timeSS.fail())
    {
        time_t rawtime;
        struct tm * timeinfo;
        
        time (&rawtime);
        timeinfo = localtime(&rawtime);

        dt.year = timeinfo->tm_year + 1900;
        dt.month = timeinfo->tm_mon + 1;
        dt.day = timeinfo->tm_mday;
        dt.hour = timeinfo->tm_hour;
        dt.min = timeinfo->tm_min;
        dt.sec = timeinfo->tm_sec;
        return is;
    }
    
    return is;
}
