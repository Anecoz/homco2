#pragma once

#include "Timestamp.h"

namespace homco2 {
namespace common {

class Interval
{
public:
  Interval();
  Interval(const Timestamp& start, const Timestamp& end);
  ~Interval() = default;

  explicit operator bool() const;

  bool intersects(const Timestamp& timestamp) const;
private:
  Timestamp _start;
  Timestamp _end;
};

}
}