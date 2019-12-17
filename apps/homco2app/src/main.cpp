#include "QmlAdapter.h"

#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickView>

#include <iostream>

int main(int argc, char* argv[])
{
  QGuiApplication app(argc, argv);

  QQuickView view;  
  homco2::app::QmlAdapter adapter;

  view.engine()->rootContext()->setContextProperty("adapter", &adapter);

  view.connect(view.engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);
  view.setSource(QUrl("qrc:/homco2/main.qml"));

  if (view.status() == QQuickView::Error) {
    return -1;
  }

  view.setResizeMode(QQuickView::SizeRootObjectToView);
  view.show();

  adapter.init();
  return app.exec();
}