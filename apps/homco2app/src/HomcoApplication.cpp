#include "HomcoApplication.h"

#include "QmlAdapter.h"
#include "RestClient.h"
#include "ChannelDataObject.h"

#include <QQmlEngine>
#include <QQmlContext>
#include <QGuiApplication>
#include <QQuickView>
#include <QList>
#include <QtWidgets/QAction>
#include <QtWidgets/QShortcut>

namespace homco2 {
namespace app {

HomcoApplication::HomcoApplication()
  : QObject()
  , _view(nullptr)
{}

HomcoApplication::~HomcoApplication()
{}

void HomcoApplication::setupChannelDataObjects()
{
  common::ChannelState state;
  state._master = true;
  state._state = false;
  state._overridden = false;

  QList<QObject*> qlist;
  {
    std::lock_guard<std::mutex> lock(_channelMutex);
    for (unsigned i = 0; i < 8; ++i) {
      auto channelData = new ChannelDataObject();
      channelData->updateState(state);
      _channelDataObjects.emplace_back(channelData);
      qlist.append(channelData);
    }
  }  

  _view->rootContext()->setContextProperty("channelModel", QVariant::fromValue(qlist));
}

bool HomcoApplication::setState(common::ChannelState channelState)
{
  // Update model, will trigger and update in QML aswell
  std::cout << "sub callback" << std::endl;
  std::lock_guard<std::mutex> lock(_channelMutex);
  _channelDataObjects[channelState._id]->updateState(channelState);
  return true;
}

bool HomcoApplication::setOverride(common::ChannelId id, bool state)
{
  _restClient->setOverride(id, state);
  return true;
}

bool HomcoApplication::setMaster(common::ChannelId id, bool state)
{
  _restClient->setMaster(id, state);
  return true;
}

void HomcoApplication::reloadQml()
{
  _view->engine()->clearComponentCache();
}

int HomcoApplication::run(int argc, char* argv[])
{
  QGuiApplication app(argc, argv);

  _view = new QQuickView();

  _adapter = std::make_unique<QmlAdapter>(this);
  _view->engine()->rootContext()->setContextProperty("adapter", _adapter.get());

  setupChannelDataObjects();

  // TODO: Sit and wait until we can properly initialize the rest client.
  //       Probably want to load GUI anyways though.
  _restClient = std::make_unique<RestClient>(this);
  _restClient->init();

  _view->connect(_view->engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);
  _view->setSource(QUrl("apps/homco2app/src/loader.qml"));
  _view->setMinimumHeight(500);
  _view->setMinimumWidth(500);

  if (_view->status() == QQuickView::Error) {
    return -1;
  }

  _view->engine()->rootContext()->setContextProperty("app", this);

  auto action = new QAction(this);
  action->setShortcut(Qt::Key_F5);
  connect(action, &QAction::triggered, [this](){
    reloadQml();
  });

  _view->setResizeMode(QQuickView::SizeRootObjectToView);
  _view->show();

  return app.exec();
}

}
}