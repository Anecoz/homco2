#include "QmlAdapter.h"

#include <iostream>

namespace homco2 {
namespace app {

QmlAdapter::QmlAdapter(MasterSetCb masterSetCb, OverrideSetCb overrideSetCb)
  : QObject()
  , _masterSetCb(masterSetCb)
  , _overrideSetCb(overrideSetCb)
{}

void QmlAdapter::onMasterSwitched(int id, bool state)
{
  std::cout << "Master switch, id: " << std::to_string(id) << ", state: " << std::to_string(state) << std::endl;
  _masterSetCb(static_cast<common::ChannelId>(id), state);
}

void QmlAdapter::onOverriddenSwitched(int id, bool state)
{
  std::cout << "Override switch, id: " << std::to_string(id) << ", state: " << std::to_string(state) << std::endl;
  _overrideSetCb(static_cast<common::ChannelId>(id), state);
}

}
}