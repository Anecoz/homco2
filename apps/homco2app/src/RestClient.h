#pragma once

#include "../common/ChannelState.h"
#include "../common/IChannelAdapter.h"

#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>

#include <functional>
#include <string>

namespace homco2 {
namespace app {

class RestClient
{
public:
  RestClient(common::IChannelAdapter* channelAdapter);
  ~RestClient();

  void init();

  void setMaster(common::ChannelId id, bool state);
  void setOverride(common::ChannelId id, bool state);

private:
  void setupSubscriptionListener();
  void requestInitialState();
  void subCallback(web::http::http_request req);

  std::string _baseUri;

  web::http::client::http_client _client;
  web::http::experimental::listener::http_listener _subscriptionListener;

  common::IChannelAdapter* _channelAdapter;
};

}
}