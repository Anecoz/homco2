#pragma once

#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>

#include <QObject>

#include <string>

namespace homco2 {
namespace app {

class QmlAdapter : public QObject
{
  Q_OBJECT
public:
  QmlAdapter();

  void init();

public slots:
  void onTest();

signals:
  void channelUpdated(int id, bool master, bool overridden);

private:
  void subCallback(web::http::http_request req);

  std::string _baseUri;

  web::http::client::http_client _client;
  web::http::experimental::listener::http_listener _subscriptionListener;
};

}
}