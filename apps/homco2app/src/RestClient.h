#pragma once

#include "../common/ChannelState.h"

#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>

#include <functional>
#include <string>

namespace homco2 {
namespace app {

typedef std::function<void(common::ChannelState)> ChannelStateCallback;

class RestClient
{
public:
  RestClient(ChannelStateCallback cb);
  ~RestClient();

  void init();

private:
  void setupSubscriptionListener();
  void requestInitialState();
  void subCallback(web::http::http_request req);

  std::string _baseUri;

  web::http::client::http_client _client;
  web::http::experimental::listener::http_listener _subscriptionListener;

  ChannelStateCallback _stateCb;
};

}
}