#pragma once

#include "../common/ChannelId.h"

#include <QObject>

#include <functional>

namespace homco2 {
namespace app {

typedef std::function<void(common::ChannelId, bool)> MasterSetCb;
typedef std::function<void(common::ChannelId, bool)> OverrideSetCb;

class QmlAdapter : public QObject
{
  Q_OBJECT
public:
  QmlAdapter(MasterSetCb, OverrideSetCb);
  ~QmlAdapter() = default;

public slots:
  void onMasterSwitched(int id, bool state);
  void onOverriddenSwitched(int id, bool state);

private:
  MasterSetCb _masterSetCb;
  OverrideSetCb _overrideSetCb;
};

}
}