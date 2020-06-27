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
  Q_PROPERTY(int id READ id NOTIFY idChanged)
  Q_PROPERTY(QString testNextOn READ testNextOn NOTIFY testNextOnChanged)

public:
  ChannelDataObject();
  virtual ~ChannelDataObject();

  void updateState(common::ChannelState state);

signals:
  void masterChanged(bool);
  void overriddenChanged(bool);
  void stateChanged(bool);
  void idChanged(int);
  void testNextOnChanged(QString);

private:
  void setMaster(bool);
  void setOverridden(bool);

  QString testNextOn() const;
  int id() const;
  bool isMaster() const;
  bool isOverridden() const;
  bool isState() const;

  common::ChannelState _currentState;
};

}
}