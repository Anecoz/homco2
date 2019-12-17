#pragma once

#include <QObject>

namespace homco2 {
namespace app {

class QmlAdapter : public QObject
{
  Q_OBJECT
public:
  QmlAdapter();

  void init();

public slots:
  void onTest();

signals:
  void created();

};

}
}