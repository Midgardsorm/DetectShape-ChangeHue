#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <huecontroller.h>

int main(int argc, char *argv[]) {

  HueController controller{};

  QApplication app(argc, argv);
  QQmlApplicationEngine engine{};

  engine.rootContext()->setContextProperty("HueController", &controller);
  engine.load("qrc:/main.qml");

  return app.exec();;
}
