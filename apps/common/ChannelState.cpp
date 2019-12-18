#include "ChannelState.h"

namespace homco2 {
namespace common {

ChannelState channelStateFromJson(web::json::value json)
{
  ChannelState channelState;
  channelState._id = json[utility::conversions::to_string_t("id")].as_integer();
  channelState._state = json[utility::conversions::to_string_t("state")].as_bool();
  channelState._overridden = json[utility::conversions::to_string_t("overridden")].as_bool();
  channelState._master = json[utility::conversions::to_string_t("master")].as_bool();

  return channelState;
}

web::json::value channelStateToJson(const ChannelState& channelState)
{
  auto topObject = web::json::value::object();
  topObject[utility::conversions::to_string_t("id")] = web::json::value::number(channelState._id);
  topObject[utility::conversions::to_string_t("state")] = web::json::value::boolean(channelState._state);
  topObject[utility::conversions::to_string_t("master")] = web::json::value::boolean(channelState._master);
  topObject[utility::conversions::to_string_t("overridden")] = web::json::value::boolean(channelState._overridden);

  return topObject;
}

}
}