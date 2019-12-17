#include "RestHandler.h"

#include <cpprest/asyncrt_utils.h>
#include <cpprest/http_client.h>

#include <algorithm>
#include <iostream>
#include <vector>

namespace homco2 {
namespace server {

RestHandler::RestHandler(
  unsigned port,
  const std::vector<common::ChannelId>& channels,
  ChannelStateCallback channelStateCb,
  ChannelMasterStateCallback channelMasterStateCb,
  ChannelOverrideStateCallback channelOverrideStateCb,
  ChannelOverrideCallback channelOverrideCb,
  ChannelMasterCallback channelMasterCb,
  ChannelSetTimerCallback channelSetTimerCb,
  ChannelTimerStateCallback channelTimerStateCb)
  : _channels(channels)
  , _uri("http://localhost:" + std::to_string(port) + "/api")
  , _channelStateCb(channelStateCb)
  , _channelMasterStateCb(channelMasterStateCb)
  , _channelOverrideStateCb(channelOverrideStateCb)
  , _channelOverrideCb(channelOverrideCb)
  , _channelMasterCb(channelMasterCb)
  , _channelSetTimerCb(channelSetTimerCb)
  , _channelTimerStateCb(channelTimerStateCb)
{}

bool RestHandler::init()
{  
  // Setup a base listener for the root uri
  _baseListener = web::http::experimental::listener::http_listener(utility::conversions::to_string_t(_uri));
  _baseListener.support(web::http::methods::GET, std::bind(&RestHandler::baseGet, this, std::placeholders::_1));
  _baseListener.support(web::http::methods::POST, std::bind(&RestHandler::subscribePost, this, std::placeholders::_1));

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

void RestHandler::notifySubscribers(common::ChannelState channelState)
{
  if (_subscriberUris.empty()) {
    return;
  }

  for (auto it = _subscriberUris.begin(); it != _subscriberUris.end();) {
    web::http::client::http_client client(utility::conversions::to_string_t(*it));
    auto topObject = web::json::value::object();
    topObject[utility::conversions::to_string_t("id")] = web::json::value::number(channelState._id);
    topObject[utility::conversions::to_string_t("state")] = web::json::value::boolean(channelState._state);
    topObject[utility::conversions::to_string_t("master")] = web::json::value::boolean(channelState._master);
    topObject[utility::conversions::to_string_t("overridden")] = web::json::value::boolean(channelState._overridden);

    try {
      std::cout << "Notifying uri " << *it << "... ";
      auto response = client.request(web::http::methods::POST, U("/"), topObject).get();
      std::cout << "Done." << std::endl;
      ++it;
    }
    catch (std::exception& e) {
      std::cout << "Exception when notifying subscriber (" + *it + "): " << e.what() << std::endl;
      // Drop the uri
      it = _subscriberUris.erase(it);
    }
  }
}

void RestHandler::baseGet(web::http::http_request req)
{
  // TODO: Return list of channels
  req.reply(200);
}

void RestHandler::subscribePost(web::http::http_request req)
{
  auto relativeUri = utility::conversions::to_utf8string(req.relative_uri().to_string());

  if (relativeUri == "/subscribe") {
    auto address = utility::conversions::to_utf8string(req.remote_address());
    if (address[0] == ':') {
      // Assume it is an IPV6-addr, and needs brackets
      address = "[" + address + "]";
    }
    address = std::string("http://") + address + std::string(":54321");
    if (std::find(_subscriberUris.begin(), _subscriberUris.end(), address) == _subscriberUris.end()) {
      _subscriberUris.emplace_back(address);
    }    

    // Also notify the poor subscriber of his own address.
    auto json = web::json::value::object();
    json[utility::conversions::to_string_t("addr")] = web::json::value::string(utility::conversions::to_string_t(address));
    req.reply(201, json);
  }
  req.reply(404);
}

void RestHandler::handleGet(web::http::http_request req, common::ChannelId channel)
{
  // relativeUri will be e.g. "/override" or just "/"
  auto relativeUri = utility::conversions::to_utf8string(req.relative_uri().to_string());
  if (relativeUri == "/") {
    auto state = _channelStateCb(channel);
    auto responseJson = web::json::value::boolean(state);
    req.reply(200, responseJson);
  }
  else if (relativeUri == "/master") {
    auto state = _channelMasterStateCb(channel);
    auto responseJson = web::json::value::boolean(state);
    req.reply(200, responseJson);
  }
  else if (relativeUri == "/override") {
    auto state = _channelOverrideStateCb(channel);
    auto responseJson = web::json::value::boolean(state);
    req.reply(200, responseJson);
  }
  else if (relativeUri == "/timer") {
    auto intervals = _channelTimerStateCb(channel);

    if (!intervals.empty()) {
      auto dayArray = web::json::value::array();
      auto timeOnArray = web::json::value::array();
      auto timeOffArray = web::json::value::array();

      for (unsigned i = 0; i < intervals.size(); ++i) {
        dayArray.as_array()[i] = common::indexFromDay(intervals.at(i)._day);
      }

      timeOnArray.as_array()[0] = intervals[0]._clockpointOn._hour;
      timeOnArray.as_array()[1] = intervals[0]._clockpointOn._minute;
      
      timeOffArray.as_array()[0] = intervals[0]._clockpointOff._hour;
      timeOffArray.as_array()[1] = intervals[0]._clockpointOff._minute;
      
      auto topObject = web::json::value::object();
      topObject[utility::conversions::to_string_t("dayInterval")] = dayArray;
      topObject[utility::conversions::to_string_t("onTime")] = timeOnArray;
      topObject[utility::conversions::to_string_t("offTime")] = timeOffArray;

      req.reply(200, topObject);
      return;
    }
    req.reply(200);    
  }
}

void RestHandler::handlePost(web::http::http_request req, common::ChannelId channel)
{
  // relativeUri will be e.g. "/override" or just "/"
  auto relativeUri = utility::conversions::to_utf8string(req.relative_uri().to_string());

  if (relativeUri == "/") {
    // Set a new timer entry for the channel, extract information from body.
    auto value = req.extract_json().get();
    if (value.is_object()) {
      auto topObject = value.as_object();

      // On time
      auto onTimeArray = topObject.at(utility::conversions::to_string_t("onTime")).as_array();
      if (onTimeArray.size() != 2) {
        req.reply(400);
        return;
      }
      unsigned onHour = onTimeArray[0].as_integer();
      unsigned onMinute = onTimeArray[1].as_integer();

      // Off time
      auto offTimeArray = topObject.at(utility::conversions::to_string_t("offTime")).as_array();
      if (offTimeArray.size() != 2) {
        req.reply(400);
        return;
      }
      unsigned offHour = offTimeArray[0].as_integer();
      unsigned offMinute = offTimeArray[1].as_integer();
      
      // Day array
      std::vector<common::WeekdayInterval> intervals;
      auto dayArray = topObject.at(utility::conversions::to_string_t("dayInterval")).as_array();
      intervals.reserve(dayArray.size());
      for (auto& day: dayArray) {
        common::WeekdayInterval interval(common::dayFromIndex(day.as_integer()), {onHour, onMinute}, {offHour, offMinute});
        intervals.emplace_back(std::move(interval));
      }

      _channelSetTimerCb(channel, intervals);

      req.reply(201);
      return;
    }
  }
  else if (relativeUri == "/override") {
    // Override the state of the channel with whatever is requested.
    auto value = req.extract_json().get();
    if (value.is_object()) {
      auto val = value.as_object().at(utility::conversions::to_string_t("state")).as_bool();
      _channelOverrideCb(channel, val);
      req.reply(200);
      return;
    }
    else {
      req.reply(500);
      return;
    }
  }
  else if (relativeUri == "/master") {
    auto value = req.extract_json().get();
    if (value.is_object()) {
      auto val = value.as_object().at(utility::conversions::to_string_t("state")).as_bool();
      _channelMasterCb(channel, val);
      req.reply(200);
    }
    else {
      req.reply(500);
    }
  }

  req.reply(500);
}

}
}