#include "ChannelDataObject.h"

namespace homco2 {
namespace app {

ChannelDataObject::ChannelDataObject()
  : QObject()
{}

ChannelDataObject::~ChannelDataObject()
{}

void ChannelDataObject::updateState(common::ChannelState state)
{
  if (state._state != _currentState._state) {
    emit stateChanged(state._state);
  }
  if (state._master != _currentState._master) {
    emit masterChanged(state._master);
  }
  if (state._overridden != _currentState._overridden) {
    emit overriddenChanged(state._overridden);
  }

  std::cout << "Updating state, size of weekday intervals is: " << std::to_string(state._intervals.size()) << std::endl;

  _currentState = state;
}

void ChannelDataObject::setMaster(bool val)
{
  // TODO, update upstream etc
}

void ChannelDataObject::setOverridden(bool val)
{
  // TODO, update upstream etc
}

QString ChannelDataObject::testNextOn() const
{
  if (_currentState._intervals.empty()) {
    return "No timer set";
  }

  std::string out = 
  std::to_string(_currentState._intervals[0]._clockpointOn._hour) + ":" +
  std::to_string(_currentState._intervals[0]._clockpointOn._minute);

  return QString(out.c_str());
}

int ChannelDataObject::id() const 
{
  return static_cast<int>(_currentState._id);
}

bool ChannelDataObject::isMaster() const
{
  return _currentState._master;
}

bool ChannelDataObject::isOverridden() const
{
  return _currentState._overridden;
}

bool ChannelDataObject::isState() const
{
  return _currentState._state;
}

}
}