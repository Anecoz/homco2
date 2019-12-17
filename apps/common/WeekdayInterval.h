#pragma once

#include <string>

namespace homco2 {
namespace common {

enum class Day
{
  Monday,
  Tuesday,
  Wednesday,
  Thursday,
  Friday,
  Saturday,
  Sunday
};

struct Clockpoint
{
  Clockpoint()
    : _hour(100)
    , _minute(100)
  {}

  Clockpoint(unsigned hour, unsigned minute)
    : _hour(hour)
    , _minute(minute)
  {}

  explicit operator bool() const 
  {
    return _hour != 100 || _minute != 100;
  }

  unsigned _hour;
  unsigned _minute;
};

class WeekdayInterval
{
public:
  WeekdayInterval();
  WeekdayInterval(Day day, Clockpoint clockpointOn, Clockpoint clockpointOff);
  ~WeekdayInterval() = default;

  explicit operator bool() const;

  bool activeNow() const;

  std::string info() const;

  Day _day;
  Clockpoint _clockpointOn;
  Clockpoint _clockpointOff;
};

// 0-6, 0 = monday, 6 = sunday.
Day dayFromIndex(unsigned index);
unsigned indexFromDay(Day day);

}
}