#pragma once

#include "../common/ChannelState.h"

#include <QObject>

namespace homco2 {
namespace app {

class ChannelDataObject : public QObject
{
  Q_OBJECT

  Q_PROPERTY(bool master READ isMaster WRITE setMaster NOTIFY masterChanged)
  Q_PROPERTY(bool overridden READ isOverridden WRITE setOverridden NOTIFY overriddenChanged)
  Q_PROPERTY(bool state READ isState NOTIFY stateChanged)

public:
  ChannelDataObject();
  virtual ~ChannelDataObject();

  void updateState(common::ChannelState state);

signals:
  void masterChanged(bool);
  void overriddenChanged(bool);
  void stateChanged(bool);

private:
  void setMaster(bool);
  void setOverridden(bool);

  bool isMaster() const;
  bool isOverridden() const;
  bool isState() const;

  common::ChannelState _currentState;
};

}
}