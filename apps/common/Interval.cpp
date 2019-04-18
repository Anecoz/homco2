#include "Interval.h"

namespace homco2 {
namespace common {

Interval::Interval()
  : _start()
  , _end()
{}

Interval::Interval(const Timestamp& start, const Timestamp& end)
  : _start(start)
  , _end(end)
{}

Interval::operator bool() const
{
  return _start && _end;
}

bool Interval::intersects(const Timestamp& timestamp) const
{
  return *this && _start <= timestamp && _end >= timestamp;  
}

}
}