#include "RestHandler.h"

#include <cpprest/asyncrt_utils.h>
#include <iostream>

namespace homco2 {
namespace server {

RestHandler::RestHandler(
  unsigned port,
  const std::vector<ChannelId>& channels,
  ChannelStateCallback channelStateCb,
  ChannelSetCallback channelSetCb)
  : _channels(channels)
  , _uri("http://localhost:" + std::to_string(port) + "/api")
  , _channelStateCb(channelStateCb)
  , _channelSetCb(channelSetCb)
{}

bool RestHandler::init()
{  
  // Setup a base listener for the root uri
  _baseListener = web::http::experimental::listener::http_listener(utility::conversions::to_string_t(_uri));
  _baseListener.support(web::http::methods::GET, std::bind(&RestHandler::baseGet, this, std::placeholders::_1));

  // Setup a listener for each channel, so we don't have to parse each request in a single "base" listener.
  for (const auto& channel : _channels) {
    web::http::experimental::listener::http_listener listener(utility::conversions::to_string_t(_uri + "/" + std::to_string(channel)));
    listener.support(web::http::methods::GET, std::bind(&RestHandler::handleGet, this, std::placeholders::_1, channel));
    listener.support(web::http::methods::POST, std::bind(&RestHandler::handlePost, this, std::placeholders::_1, channel));

    try {
      listener.open().then([]() {
        std::cout << "Created listener for a channel" << std::endl;
      }).wait();
    }
    catch (const std::exception & e) {
      std::cerr << "Exception caught: " << e.what() << std::endl;
      return false;
    }

    _channelListeners.emplace_back(std::move(listener));
  }

  try {
    _baseListener.open().then([]() {
      std::cout << "Listening..." << std::endl;
    }).wait();
  }
  catch (const std::exception & e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    return false;
  }

  return true;
}

void RestHandler::baseGet(web::http::http_request req)
{
  // TODO: Return list of channels
  req.reply(200);
}

void RestHandler::handleGet(web::http::http_request req, ChannelId channel)
{
  auto state = _channelStateCb(channel);

  auto responseJson = web::json::value::boolean(state);
  req.reply(200, responseJson);
}

void RestHandler::handlePost(web::http::http_request req, ChannelId channel)
{
  auto value = req.extract_json().get();
  if (value.is_object()) {
    auto val = value.as_object().at(utility::conversions::to_string_t("state")).as_bool();
    _channelSetCb(channel, val);
    req.reply(200);
  }
  else {
    req.reply(500);
  }
}

}
}