#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFontDatabase>

#include "filemodel.h"
#include "addressrange.h"
#include "interpretermodel.h"

#include "qmlglobal.h"

#include "base_interpreters.h"

int main(int argc, char *argv[])
{
    init_base_interpreters();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QFontDatabase fontDatabase;
    fontDatabase.addApplicationFont(":/fonts/CamingoCode.ttf");

    qmlRegisterType<FileModel>       ("ReverseEngi", 1, 0, "FileModel");
    qmlRegisterType<AddressRange>    ("ReverseEngi", 1, 0, "AddressRange");
    qmlRegisterType<InterpreterModel>("ReverseEngi", 1, 0, "InterpreterModel");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("G", new QmlGlobal);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
