#pragma once

#include "../common/ChannelState.h"
#include "../common/IChannelAdapter.h"

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

class HomcoApplication : public QObject, public common::IChannelAdapter
{
  Q_OBJECT

public:
  HomcoApplication();
  ~HomcoApplication();

  int run(int argc, char* argv[]);

  bool setState(common::ChannelState state) override;
  bool setMaster(common::ChannelId id, bool val) override;
  bool setOverride(common::ChannelId id, bool val) override;

public slots:
  void reloadQml();

private:
  void setupChannelDataObjects();

  QQuickView* _view;

  std::unique_ptr<QmlAdapter> _adapter;
  std::unique_ptr<RestClient> _restClient;

  std::vector<ChannelDataObject*> _channelDataObjects;
  std::mutex _channelMutex;
};

}
}