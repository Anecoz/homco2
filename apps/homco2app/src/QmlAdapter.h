#pragma once

#include "../common/ChannelId.h"
#include "../common/WeekdayInterval.h"
#include "../common/IChannelAdapter.h"

#include <QObject>

#include <functional>

namespace homco2 {
namespace app {

class QmlAdapter : public QObject
{
  Q_OBJECT
public:
  QmlAdapter(common::IChannelAdapter* channelAdapter);
  ~QmlAdapter() = default;

public slots:
  void onMasterSwitched(int id, bool state);
  void onOverriddenSwitched(int id, bool state);
  void onTimerAccept(int id, QString day, QString hour, QString minute);

private:
  common::IChannelAdapter* _channelAdapter;
};

}
}