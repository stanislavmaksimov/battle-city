#ifndef COMMON_H
#define COMMON_H

#include <QSize>
#include <QPoint>
#include <QDebug>
#include <QDateTime>

static const float SCALE = 4;
static const float BLOCK_ORIGINAL = 8;
static const float BLOCK = qRound(BLOCK_ORIGINAL * SCALE);
static const float TILE_ORIGINAL = qRound(BLOCK_ORIGINAL * 2);
static const float TILE = qRound(TILE_ORIGINAL * SCALE);
static const QString SPRITES(":/images/sprites.png");
static const QString SPRITE_RESOURCE1("image://sprites/%1");
static const QString SPRITE_RESOURCE2("image://sprites/%1/%2");

static const int BLOCKI = qRound(BLOCK);

static const QSize BLOCK_SIZE_ORIGINAL(BLOCK_ORIGINAL, BLOCK_ORIGINAL);
static const QSize BLOCK_SIZE(BLOCK, BLOCK);
static const QSize TILE_SIZE_ORIGINAL(TILE_ORIGINAL, TILE_ORIGINAL);
static const QSize TILE_SIZE(TILE, TILE);
static const QSize FIELD_SIZE(TILE * 17, TILE * 15);

static const QPoint BLOCK_POINT(BLOCK, BLOCK);
static const QPoint TILE_POINT_ORIGINAL(TILE_ORIGINAL, TILE_ORIGINAL);

static const QString TILE_NAME_TANK("tank");
static const QString TILE_NAME_FRAME("frame");
static const QString TILE_NAME_STEEL("steel");
static const QString TILE_NAME_BRICK("brick");
static const QString TILE_NAME_MASK("mask");
static const QString TILE_NAME_BASE("base");
static const QString TILE_NAME_FLAG("flag");
static const QString TILE_NAME_INVULNERABLE("invulnerable");
static const QString SPRITE_NAME_INVULNERABLE(SPRITE_RESOURCE1.arg(TILE_NAME_INVULNERABLE));

static const qint64 SHOT_INTERVAL = 200;
static const qint64 BEHAVIOUR_INTERVAL = 1000 * 23;
static const qint64 RESPAWN_INTERVAL = BEHAVIOUR_INTERVAL / 8;

enum class Direction
{
    Up      = 0,
    Left    = 1,
    Down    = 2,
    Right   = 3
};
Q_DECLARE_FLAGS(Directions, Direction)

int random(int max = 255);

template <typename T> QPoint toPoint(const T&);

#define LOG qDebug() << Q_FUNC_INFO

#endif // COMMON_H
