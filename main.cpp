#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFontDatabase>

#include "filedocument.h"
#include "filemodel.h"
#include "addressrange.h"
#include "interpretermodel.h"
#include "base_interpreters.h"
#include "qmlglobal.h"

int main(int argc, char *argv[])
{
    init_base_interpreters();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QFontDatabase fontDatabase;
    fontDatabase.addApplicationFont(":/fonts/CamingoCode.ttf");

    qmlRegisterType<FileDocument>    ("ReverseEngi", 1, 0, "FileDocument");
    qmlRegisterType<FileModel>       ("ReverseEngi", 1, 0, "FileModel");
    qmlRegisterType<AddressRange>    ("ReverseEngi", 1, 0, "AddressRange");
    qmlRegisterType<AreaStyle>       ("ReverseEngi", 1, 0, "AreaStyle");
    qmlRegisterType<InterpreterModel>("ReverseEngi", 1, 0, "InterpreterModel");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("G", new QmlGlobal);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
