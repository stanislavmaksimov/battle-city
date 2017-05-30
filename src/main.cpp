#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "tile.h"
#include "engine.h"
#include "inputhandler.h"
#include "imageprovider.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qRegisterMetaType<Tile>("Tile");
    qRegisterMetaType<QList<Tile>>("QList<Tile>");
    qRegisterMetaType<QList<Qt::Key>>("QList<Qt::Key>");

    qmlRegisterType<Engine>("BattleCity", 1, 0, "Engine");
    qmlRegisterType<InputHandler>("BattleCity", 1, 0, "InputHandler");

    QQmlApplicationEngine engine;
    engine.addImageProvider(QLatin1String("sprites"), new ImageProvider());
    if (QQmlContext* context = engine.rootContext()) {
        context->setContextProperty("TILE", TILE);
        context->setContextProperty("BLOCK", BLOCK);
        context->setContextProperty("TILE_NAME_TANK", TILE_NAME_TANK);
        context->setContextProperty("TILE_NAME_BRICK", TILE_NAME_BRICK);
        context->setContextProperty("TILE_NAME_FRAME", TILE_NAME_FRAME);
        context->setContextProperty("TILE_NAME_STEEL", TILE_NAME_STEEL);
        context->setContextProperty("TILE_NAME_MASK", TILE_NAME_MASK);
        context->setContextProperty("SPRITE_NAME_INVULNERABLE", SPRITE_NAME_INVULNERABLE);
    }
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}
