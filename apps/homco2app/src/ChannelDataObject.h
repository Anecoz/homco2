#pragma once

#include "../common/ChannelState.h"

#include <QObject>
#include <QString>

namespace homco2 {
namespace app {

class ChannelDataObject : public QObject
{
  Q_OBJECT

  Q_PROPERTY(bool master READ isMaster WRITE setMaster NOTIFY masterChanged)
  Q_PROPERTY(bool overridden READ isOverridden WRITE setOverridden NOTIFY overriddenChanged)
  Q_PROPERTY(bool state READ isState NOTIFY stateChanged)
  Q_PROPERTY(QString id READ id NOTIFY idChanged)

public:
  ChannelDataObject();
  virtual ~ChannelDataObject();

  void updateState(common::ChannelState state);

signals:
  void masterChanged(bool);
  void overriddenChanged(bool);
  void stateChanged(bool);
  void idChanged(QString);

private:
  void setMaster(bool);
  void setOverridden(bool);

  QString id() const;
  bool isMaster() const;
  bool isOverridden() const;
  bool isState() const;

  common::ChannelState _currentState;
};

}
}