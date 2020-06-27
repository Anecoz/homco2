#include "WeekdayInterval.h"

#include <iostream>
#include <chrono>

namespace homco2 {
namespace common {

Day dayFromIndex(unsigned index)
{
  switch (index) {
    case 0:
      return Day::Monday;
    case 1:
      return Day::Tuesday;
    case 2:
      return Day::Wednesday;
    case 3:
      return Day::Thursday;
    case 4:
      return Day::Friday;
    case 5:
      return Day::Saturday;
    case 6:
      return Day::Monday;
    default:
      return Day::Monday;
  }
}

unsigned indexFromDay(Day day) {
  switch (day) {
    case Day::Monday:
      return 0;
    case Day::Tuesday:
      return 1;
    case Day::Wednesday:
      return 2;
    case Day::Thursday:
      return 3;
    case Day::Friday:
      return 4;
    case Day::Saturday:
      return 5;
    case Day::Sunday:
      return 6;
    default:
      return 0;
  }
}

WeekdayInterval::WeekdayInterval()
  : _day(Day::Monday)
  , _clockpointOn()
  , _clockpointOff()
{}

WeekdayInterval::WeekdayInterval(Day day, Clockpoint clockpointOn, Clockpoint clockpointOff)
  : _day(day)
  , _clockpointOn(clockpointOn)
  , _clockpointOff(clockpointOff)
{}

static Day wDayToDay(int wDay)
{
  switch (wDay) {
    case 0:
      return Day::Sunday;
    case 1:
      return Day::Monday;
    case 2:
      return Day::Tuesday;
    case 3:
      return Day::Wednesday;
    case 4:
      return Day::Thursday;
    case 5:
      return Day::Friday;
    case 6:
      return Day::Saturday;
    default:
      return Day::Monday;
  }
}

bool WeekdayInterval::activeNow() const
{
  auto now = std::chrono::system_clock::now();

  time_t tt = std::chrono::system_clock::to_time_t(now);
  tm local_tm = *localtime(&tt);

  auto currentDay = wDayToDay(local_tm.tm_wday);
  auto currentHour = local_tm.tm_hour;
  auto currentMinute = local_tm.tm_min;
  auto currentSecond = local_tm.tm_sec;

  if (currentDay == _day) {
    // Convert our internal clockpoints to seconds after midnight, easier to handle.
    auto secondsOn = _clockpointOn._minute * 60 + _clockpointOn._hour * 60 * 60;
    auto secondsOff = _clockpointOff._minute * 60 + _clockpointOff._hour * 60 * 60;

    auto currentSeconds = static_cast<unsigned>(currentSecond + currentMinute * 60 + currentHour * 60 * 60);
    if (currentSeconds >= secondsOn && currentSeconds <= secondsOff) {
      return true;
    }
  }

  return false;
}

WeekdayInterval::operator bool() const
{
  return _clockpointOn && _clockpointOff;
}

std::string WeekdayInterval::info() const
{
  return std::string("Weekday: ") + std::to_string(static_cast<int>(_day)) + 
    std::string(", On: ") + std::to_string(_clockpointOn._hour) + ":" + std::to_string(_clockpointOn._minute) +
    std::string(", Off: ") + std::to_string(_clockpointOff._hour) + ":" + std::to_string(_clockpointOff._minute);
}

std::vector<WeekdayInterval> weekdaysFromJson(web::json::value json)
{
  std::vector<WeekdayInterval> output;

  if (json.is_object()) {
    auto topObject = json.as_object();

    // On time
    auto onTimeArray = topObject.at(utility::conversions::to_string_t("onTime")).as_array();
    if (onTimeArray.size() != 2) {
      return output;
    }
    unsigned onHour = onTimeArray[0].as_integer();
    unsigned onMinute = onTimeArray[1].as_integer();

    // Off time
    auto offTimeArray = topObject.at(utility::conversions::to_string_t("offTime")).as_array();
    if (offTimeArray.size() != 2) {
      return output;
    }
    unsigned offHour = offTimeArray[0].as_integer();
    unsigned offMinute = offTimeArray[1].as_integer();
    
    // Day array
    auto dayArray = topObject.at(utility::conversions::to_string_t("dayInterval")).as_array();
    output.reserve(dayArray.size());
    for (auto& day: dayArray) {
      common::WeekdayInterval interval(common::dayFromIndex(day.as_integer()), {onHour, onMinute}, {offHour, offMinute});
      output.emplace_back(std::move(interval));
    }
  }

  return output;
}

web::json::value weekdaysToJson(const std::vector<WeekdayInterval>& weekdays)
{
  if (weekdays.empty()) {
    return web::json::value();
  }

  auto dayArray = web::json::value::array();
  auto timeOnArray = web::json::value::array();
  auto timeOffArray = web::json::value::array();

  for (unsigned i = 0; i < weekdays.size(); ++i) {
    dayArray.as_array()[i] = common::indexFromDay(weekdays.at(i)._day);
  }

  timeOnArray.as_array()[0] = weekdays[0]._clockpointOn._hour;
  timeOnArray.as_array()[1] = weekdays[0]._clockpointOn._minute;
  
  timeOffArray.as_array()[0] = weekdays[0]._clockpointOff._hour;
  timeOffArray.as_array()[1] = weekdays[0]._clockpointOff._minute;
  
  auto topObject = web::json::value::object();
  topObject[utility::conversions::to_string_t("dayInterval")] = dayArray;
  topObject[utility::conversions::to_string_t("onTime")] = timeOnArray;
  topObject[utility::conversions::to_string_t("offTime")] = timeOffArray;

  return topObject;
}

}
}