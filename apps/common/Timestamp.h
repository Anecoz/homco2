#pragma once

#include <cstdint>

namespace homco2 {
namespace common {

class Timestamp
{
public:
  Timestamp();
  Timestamp(std::int64_t epochSeconds);
  ~Timestamp() = default;

  static Timestamp now();

  explicit operator bool() const;

  bool operator==(const Timestamp& other) const;
  bool operator<(const Timestamp& other) const;
  bool operator>(const Timestamp& other) const;
  bool operator<=(const Timestamp& other) const;
  bool operator>=(const Timestamp& other) const;

private:
  std::int64_t _sec;
};

}
}