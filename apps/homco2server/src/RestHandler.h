#pragma once

#include "ChannelId.h"
#include "../common/WeekdayInterval.h"

#include <cpprest/http_listener.h>

#include <functional>
#include <string>

/*
API:
/ GET: List of channelIds available
/<channel>          GET: Return boolean representing if channel is currently on or off
/<channel>/timer    GET: Return json array with timer configuration (week day intervals, time on and time off)
/<channel>          POST: Set new timer interval. Body contains day intervals, time on and time off (minute resolution)
/<channel>/override POST: Override, set state on or off (true or false in body)
/<channel>/override GET: Return override status as bool
/<channel>/master   POST: Master switch (true or false in body)
/<channel>/master   GET: Return master status as bool
*/

namespace homco2 {
namespace server {

typedef std::function<bool(ChannelId)> ChannelStateCallback;
typedef std::function<bool(ChannelId)> ChannelMasterStateCallback;
typedef std::function<bool(ChannelId)> ChannelOverrideStateCallback;
typedef std::function<bool(ChannelId, bool)> ChannelOverrideCallback;
typedef std::function<bool(ChannelId, bool)> ChannelMasterCallback;
typedef std::function<bool(ChannelId, std::vector<common::WeekdayInterval>)> ChannelSetTimerCallback;
typedef std::function<std::vector<common::WeekdayInterval>(ChannelId)> ChannelTimerStateCallback;

class RestHandler
{
public:
  RestHandler(
    unsigned port,
    const std::vector<ChannelId>& channels,
    ChannelStateCallback,
    ChannelMasterStateCallback,
    ChannelOverrideStateCallback,
    ChannelOverrideCallback,
    ChannelMasterCallback,
    ChannelSetTimerCallback,
    ChannelTimerStateCallback);
  ~RestHandler() = default;

  bool init();

private:
  // To get a list of channels.
  void baseGet(web::http::http_request req);

  // To poll current state of a channel.
  void handleGet(web::http::http_request req, ChannelId channel);

  // For setting override state, setting a new timer etc.
  void handlePost(web::http::http_request req, ChannelId channel);

  std::vector<ChannelId> _channels;
  std::string _uri;
  web::http::experimental::listener::http_listener _baseListener;
  std::vector<web::http::experimental::listener::http_listener> _channelListeners;

  ChannelStateCallback _channelStateCb;
  ChannelMasterStateCallback _channelMasterStateCb;
  ChannelOverrideStateCallback _channelOverrideStateCb;
  ChannelOverrideCallback _channelOverrideCb;
  ChannelMasterCallback _channelMasterCb;
  ChannelSetTimerCallback _channelSetTimerCb;
  ChannelTimerStateCallback _channelTimerStateCb;
};

}
}