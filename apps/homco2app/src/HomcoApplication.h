#pragma once

#include "../common/ChannelState.h"

#include <QObject>
#include <QQuickView>

#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace homco2 {
namespace app {

class QmlAdapter;
class ChannelDataObject;
class RestClient;

class HomcoApplication : public QObject
{
  Q_OBJECT

public:
  HomcoApplication();
  ~HomcoApplication();

  int run(int argc, char* argv[]);

public slots:
  void reloadQml();

private:
  void setupChannelDataObjects();
  void subCallback(common::ChannelState channelState);
  void masterSetCallback(common::ChannelId id, bool state);
  void overrideSetCallback(common::ChannelId id, bool state);

  QQuickView* _view;

  std::unique_ptr<QmlAdapter> _adapter;
  std::unique_ptr<RestClient> _restClient;

  std::vector<ChannelDataObject*> _channelDataObjects;
  std::mutex _channelMutex;
};

}
}