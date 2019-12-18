#include "QmlAdapter.h"

namespace homco2 {
namespace app {

QmlAdapter::QmlAdapter()
  : QObject()
{
}

void QmlAdapter::init()
{
  
}

void QmlAdapter::onTest()
{
  // Create a http client and ask server for state of channel 0
  /*web::http::client::http_client client(utility::conversions::to_string_t("http://localhost:12345/api/0"));
  client.request(web::http::methods::GET, U("/")).then([](const web::http::http_response& response) {
    auto json = response.extract_json().get().as_bool();
    std::cout << "Got response: " << std::to_string(json) << std::endl;
  });*/

  /*auto topObject = web::json::value::object();
  topObject[utility::conversions::to_string_t("state")] = web::json::value::boolean(true);
  _client.request(web::http::methods::POST, U("/0/override"), topObject).then([](const web::http::http_response& response) {
    std::cout << "Got response: " << std::to_string(response.status_code()) << std::endl;
  });*/
}

}
}