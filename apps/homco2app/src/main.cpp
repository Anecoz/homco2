#include <QGuiApplication>
#include <QQmlEngine>
#include <QQuickView>

#include <iostream>

int main(int argc, char* argv[])
{
  QGuiApplication app(argc, argv);

  QQuickView view;
  view.connect(view.engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);
  view.setSource(QUrl("qrc:/homco2/main.qml"));
  if (view.status() == QQuickView::Error)
      return -1;
  view.setResizeMode(QQuickView::SizeRootObjectToView);
  view.show();
  return app.exec();
}