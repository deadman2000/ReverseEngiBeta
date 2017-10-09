#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QFontDatabase fontDatabase;
    fontDatabase.addApplicationFont(":/fonts/CamingoCode.ttf");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
