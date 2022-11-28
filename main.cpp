#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "griditemmodel.h"
#include "core.h"
#include <QQuickWindow>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    GridItemModel gridItemModel;

    QQmlContext *qml_context = engine.rootContext();
    qml_context->setContextProperty("gridItemModel", &gridItemModel);

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    Core core;

    QObject::connect(&core, &Core::newRandomBall, &gridItemModel, &GridItemModel::addNewBall);
    QObject::connect(&core, &Core::scoreUpdated, &gridItemModel, &GridItemModel::scoreUpdated);
    QObject::connect(&core, &Core::removeBall, &gridItemModel, &GridItemModel::markAsRemoveBall);
    QObject::connect(&core, &Core::setBall, &gridItemModel, &GridItemModel::setBall);
    QObject::connect(&core, &Core::gameOver, &gridItemModel, &GridItemModel::gameOver);
    QObject::connect(&gridItemModel, &GridItemModel::moveBall, &core, &Core::moveBall);
    QObject::connect(&gridItemModel, &GridItemModel::newGame, &core, &Core::newGame);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url, &core, &gridItemModel](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            QCoreApplication::exit(-1);
        }

        core.restoreGame();
        gridItemModel.updateData();
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
