#pragma once

#include "ChannelId.h"

#include <cpprest/http_listener.h>

#include <functional>
#include <string>

/*
API:
/ GET: List of channelIds available
/<channel> GET: Return boolean representing if channel is currently on or off
/<channel> POST: Set state on or off (true or false in body)
*/

namespace homco2 {
namespace server {

typedef std::function<bool(ChannelId)> ChannelStateCallback;
typedef std::function<bool(ChannelId, bool)> ChannelSetCallback;

class RestHandler
{
public:
  RestHandler(
    unsigned port,
    const std::vector<ChannelId>& channels,
    ChannelStateCallback,
    ChannelSetCallback);
  ~RestHandler() = default;

  bool init();

private:
  void baseGet(web::http::http_request req);
  void handleGet(web::http::http_request req, ChannelId channel);
  void handlePost(web::http::http_request req, ChannelId channel);

  std::vector<ChannelId> _channels;
  std::string _uri;
  web::http::experimental::listener::http_listener _baseListener;
  std::vector<web::http::experimental::listener::http_listener> _channelListeners;

  ChannelStateCallback _channelStateCb;
  ChannelSetCallback _channelSetCb;
};

}
}