#include "QmlAdapter.h"

#include <iostream>

namespace homco2 {
namespace app {

QmlAdapter::QmlAdapter(common::IChannelAdapter* channelAdapter)
  : QObject()
  , _channelAdapter(channelAdapter)
{}

void QmlAdapter::onMasterSwitched(int id, bool state)
{
  std::cout << "Master switch, id: " << std::to_string(id) << ", state: " << std::to_string(state) << std::endl;
  _channelAdapter->setMaster(static_cast<common::ChannelId>(id), state);
}

void QmlAdapter::onOverriddenSwitched(int id, bool state)
{
  std::cout << "Override switch, id: " << std::to_string(id) << ", state: " << std::to_string(state) << std::endl;
  _channelAdapter->setOverride(static_cast<common::ChannelId>(id), state);
}

void QmlAdapter::onTimerAccept(int id, QString day, QString hour, QString minute)
{
  std::cout << "Timer accept: " << day.toStdString() << ", " << std::to_string(hour.toInt()) << ":" << std::to_string(minute.toInt()) << std::endl;
  //s_intervalSetCb(static_cast<common::ChannelId>(id), )
}

}
}