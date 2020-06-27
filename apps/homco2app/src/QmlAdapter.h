#pragma once

#include "../common/ChannelId.h"
#include "../common/WeekdayInterval.h"

#include <QObject>

#include <functional>

namespace homco2 {
namespace app {

typedef std::function<void(common::ChannelId, bool)> MasterSetCb;
typedef std::function<void(common::ChannelId, bool)> OverrideSetCb;
typedef std::function<void(common::ChannelId, std::vector<common::WeekdayInterval>)> IntervalSetCb;

class QmlAdapter : public QObject
{
  Q_OBJECT
public:
  QmlAdapter(MasterSetCb, OverrideSetCb/*, IntervalSetCb*/);
  ~QmlAdapter() = default;

public slots:
  void onMasterSwitched(int id, bool state);
  void onOverriddenSwitched(int id, bool state);
  void onTimerAccept(int id, QString day, QString hour, QString minute);

private:
  MasterSetCb _masterSetCb;
  OverrideSetCb _overrideSetCb;
  //IntervalSetCb _intervalSetCb;
};

}
}