#ifndef DATETIME_H
#define DATETIME_H
#include <ctime>
#include <string>
#include <iostream>
#include <sstream>

/**
 * Models a timestamp in format YYYY-MM-DD HH:MM:SS 
 */
struct DateTime
{
  /**
   * Default constructor for current system time/date
   */
  DateTime();

  /**
   * Alternative constructor 
   */
  DateTime(int hh, int mm, int ss, int year, int month, int day);

  /**
   * Return true if the timestamp is less-than other's
   *
   * @return result of less-than comparison of timestamp
   */
  bool operator<(const DateTime& other) const;

  /**
   * Outputs the timestamp to the given ostream in format:
   *   YYYY-MM-DD HH:MM:SS
   *
   * @return the ostream passed in as an argument
   */
  friend std::ostream& operator<<(std::ostream& os, const DateTime& other);

  /**
   * Inputs the timestamp from the given istream expecting the format:
   *   YYYY-MM-DD HH:MM:SS
   *  Returns if an error is found in the format but sets the DateTime 
   *   to the current system time/date
   *   
   *
   * @return the istream passed in as an argument
   */
  friend std::istream& operator>>(std::istream& is, DateTime& dt);

  /* Public data members */
  int hour;
  int min;
  int sec;
  int year;
  int month;
  int day;
};

#endif
