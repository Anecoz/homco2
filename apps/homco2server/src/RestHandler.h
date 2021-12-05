#pragma once

#include "../common/ChannelId.h"
#include "../common/ChannelState.h"
#include "../common/WeekdayInterval.h"
#include "../common/IChannelAdapter.h"

#include <cpprest/http_listener.h>

#include <functional>
#include <string>

/*
API:
/                   GET: List of channelIds available
/subscribe          POST: Subscribe to state changes of channels (callback url in body)
/<channel>          GET: Return channel state json object
/<channel>/timer    GET: Return json array with timer configuration (week day intervals, time on and time off)
/<channel>          POST: Set new timer interval. Body contains day intervals, time on and time off (minute resolution)
/<channel>/override POST: Override, set state on or off (true or false in body)
/<channel>/override GET: Return override status as bool
/<channel>/master   POST: Master switch (true or false in body)
/<channel>/master   GET: Return master status as bool
*/

namespace homco2 {
namespace server {

class RestHandler
{
public:
  RestHandler(
    unsigned port,
    const std::vector<common::ChannelId>& channels,
    common::IChannelAdapter* channelAdapter);
  ~RestHandler() = default;

  bool init();

  void notifySubscribers(common::ChannelState channelState);

private:
  // To get a list of channels.
  void baseGet(web::http::http_request req);

  // To subscribe.
  void subscribePost(web::http::http_request req);

  // To poll current state of a channel.
  void handleGet(web::http::http_request req, common::ChannelId channel);

  // For setting override state, setting a new timer etc.
  void handlePost(web::http::http_request req, common::ChannelId channel);

  std::vector<common::ChannelId> _channels;
  std::string _uri;
  web::http::experimental::listener::http_listener _baseListener;
  std::vector<web::http::experimental::listener::http_listener> _channelListeners;

  std::vector<std::string> _subscriberUris;

  common::IChannelAdapter* _channelAdapter;
};

}
}