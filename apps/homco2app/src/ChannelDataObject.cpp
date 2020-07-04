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
  // First set the new state, so that the "changed" signals will read the correct new state.
  // But also save the old state to compare.
  common::ChannelState oldState = _currentState;
  _currentState = state;

  if (_currentState._state != oldState._state) {
    std::cout << "State changed" << std::endl;
    emit stateChanged(state._state);
  }
  if (_currentState._master != oldState._master) {
    std::cout << "Master changed" << std::endl;
    emit masterChanged(state._master);
  }
  if (_currentState._overridden != oldState._overridden) {
    std::cout << "Overridden changed" << std::endl;
    emit overriddenChanged(state._overridden);
  }

  std::cout << "Updating state: \n" << _currentState.debugStr() << std::endl;
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
    return QString("No timer set");
  }

  std::string out = 
    std::to_string(_currentState._intervals[0]._clockpointOn._hour) + ":" +
    std::to_string(_currentState._intervals[0]._clockpointOn._minute);

  std::cout << "size of intervals: " << std::to_string(_currentState._intervals.size()) << std::endl;

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