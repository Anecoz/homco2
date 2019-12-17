#include "QmlAdapter.h"

#include "../common/ChannelState.h"

#include <cpprest/asyncrt_utils.h>

namespace homco2 {
namespace app {

QmlAdapter::QmlAdapter()
  : QObject()
  , _baseUri("http://localhost:12345/api/")
  , _client(utility::conversions::to_string_t(_baseUri))
{
}

void QmlAdapter::init()
{
  // Setup subscription listener
  _client.request(web::http::methods::POST, U("/subscribe")).then([this](const web::http::http_response& response) {
    auto json = response.extract_json().get();
    auto addr = utility::conversions::to_utf8string(json[utility::conversions::to_string_t("addr")].as_string());

    _subscriptionListener = web::http::experimental::listener::http_listener(utility::conversions::to_string_t(addr));
    _subscriptionListener.support(web::http::methods::POST, std::bind(&QmlAdapter::subCallback, this, std::placeholders::_1));

    try {
      _subscriptionListener.open().then([addr]() {
        std::cout << "Listening for subscriptions on " << addr << std::endl;
      }).wait();
    }
    catch (const std::exception & e) {
      std::cerr << "Exception caught while starting subscription listener: " << e.what() << std::endl;
    }
  });
}

void QmlAdapter::subCallback(web::http::http_request req)
{
  // Parse channel state json
  common::ChannelState channelState;
  auto json = req.extract_json().get();
  channelState._id = json[utility::conversions::to_string_t("id")].as_integer();
  channelState._state = json[utility::conversions::to_string_t("state")].as_bool();
  channelState._overridden = json[utility::conversions::to_string_t("overridden")].as_bool();
  channelState._master = json[utility::conversions::to_string_t("master")].as_bool();

  emit channelUpdated(static_cast<int>(channelState._id), channelState._master, channelState._overridden);

  req.reply(200);
}

void QmlAdapter::onTest()
{
  // Create a http client and ask server for state of channel 0
  /*web::http::client::http_client client(utility::conversions::to_string_t("http://localhost:12345/api/0"));
  client.request(web::http::methods::GET, U("/")).then([](const web::http::http_response& response) {
    auto json = response.extract_json().get().as_bool();
    std::cout << "Got response: " << std::to_string(json) << std::endl;
  });*/

  auto topObject = web::json::value::object();
  topObject[utility::conversions::to_string_t("state")] = web::json::value::boolean(true);
  _client.request(web::http::methods::POST, U("/0/override"), topObject).then([](const web::http::http_response& response) {
    std::cout << "Got response: " << std::to_string(response.status_code()) << std::endl;
  });
}

}
}