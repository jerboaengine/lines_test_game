#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "griditemmodel.h"
#include "agent.h"
#include <QQuickWindow>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;



    QQmlContext *qml_context = engine.rootContext();

    Agent agent;
    GridItemModel gridItemModel;

    qml_context->setContextProperty("agent", &agent);
    qml_context->setContextProperty("gridItemModel", &gridItemModel);

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url, &agent](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            QCoreApplication::exit(-1);
        }

        agent.restoreGameRequest();
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
