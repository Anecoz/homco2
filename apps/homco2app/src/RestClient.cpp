#include "RestClient.h"

namespace homco2 {
namespace app {

RestClient::RestClient(ChannelStateCallback cb)
  : _baseUri("http://localhost:12345/api/")
  , _client(utility::conversions::to_string_t(_baseUri))
  , _stateCb(cb)
{
}

RestClient::~RestClient()
{}

void RestClient::init()
{
  setupSubscriptionListener();

  // Get initial state from server
  requestInitialState();
}

void RestClient::setMaster(common::ChannelId id, bool state)
{
  std::string req = std::string("/") + std::to_string(id) + std::string("/master");
  auto json = web::json::value::object();
  json[utility::conversions::to_string_t("state")] = web::json::value::boolean(state);

  try {
    _client.request(web::http::methods::POST, utility::conversions::to_string_t(req.c_str()), json).wait();
  }
  catch (std::exception& e) {
    std::cerr << "Caught exception while setting master: " << e.what() << std::endl;
  }
}

void RestClient::setOverride(common::ChannelId id, bool state)
{
  std::string req = std::string("/") + std::to_string(id) + std::string("/override");
  auto json = web::json::value::object();
  json[utility::conversions::to_string_t("state")] = web::json::value::boolean(state);

  try {
    _client.request(web::http::methods::POST, utility::conversions::to_string_t(req.c_str()), json).wait();
  }
  catch (std::exception& e) {
    std::cerr << "Caught exception while setting override: " << e.what() << std::endl;
  }
}

void RestClient::requestInitialState()
{
  for (unsigned i = 0; i < 8; ++i) {
    std::string req = std::string("/") + std::to_string(i);
    try {
      _client.request(web::http::methods::GET, utility::conversions::to_string_t(req.c_str())).then([this](const web::http::http_response& response) {
        common::ChannelState channelState = common::channelStateFromJson(response.extract_json().get());
        _stateCb(channelState);
      }).wait();
    }
    catch (std::exception& e) {
      std::cerr << "Caught exception while requesting initial state: " << e.what() << std::endl;
      break;
    }
  }
}

void RestClient::setupSubscriptionListener()
{
  try {
    _client.request(web::http::methods::POST, U("/subscribe")).then([this](const web::http::http_response& response) {
      auto json = response.extract_json().get();
      auto addr = utility::conversions::to_utf8string(json[utility::conversions::to_string_t("addr")].as_string());

      _subscriptionListener = web::http::experimental::listener::http_listener(utility::conversions::to_string_t(addr));
      _subscriptionListener.support(web::http::methods::POST, std::bind(&RestClient::subCallback, this, std::placeholders::_1));

      try {
        _subscriptionListener.open().then([addr]() {
          std::cout << "Listening for subscriptions on " << addr << std::endl;
        }).wait();
      }
      catch (const std::exception & e) {
        std::cerr << "Exception caught while starting subscription listener: " << e.what() << std::endl;
      }
    }).wait();
  }
  catch (std::exception& e) {
    std::cerr << "Exception caught while requesting subscription: " << e.what() << std::endl;
  }
}

void RestClient::subCallback(web::http::http_request req)
{
  // Parse channel state json
  common::ChannelState channelState = common::channelStateFromJson(req.extract_json().get());
  _stateCb(channelState);
  req.reply(200);
}

}
}