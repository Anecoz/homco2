#include "Timestamp.h"

#include <chrono>
#include <limits>

static std::int64_t g_invalidSec = std::numeric_limits<std::int64_t>::lowest();

namespace homco2 {
namespace common {

Timestamp::Timestamp()
  : _sec(g_invalidSec)
{}

Timestamp::Timestamp(std::int64_t epochSeconds)
  : _sec(epochSeconds)
{}

Timestamp Timestamp::now()
{
  const auto now = std::chrono::system_clock::now();
  const std::int64_t epochSeconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
  return Timestamp(epochSeconds);
}

Timestamp::operator bool() const
{
  return _sec != g_invalidSec;
}

bool Timestamp::operator==(const Timestamp& other) const
{
  return other._sec == _sec;
}

bool Timestamp::operator<(const Timestamp& other) const
{
  return other._sec < _sec;
}

bool Timestamp::operator>(const Timestamp& other) const
{
  return other._sec > _sec;
}

bool Timestamp::operator<=(const Timestamp& other) const
{
  return *this < other || *this == other;
}

bool Timestamp::operator>=(const Timestamp& other) const
{
  return *this > other || *this == other;
}

}
}