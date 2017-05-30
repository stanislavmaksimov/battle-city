#include "engineworker.h"
#include "common.h"
#include "tile.h"
#include "tilesloader.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QThread>
#include <QRegion>
#include <QtMath>

EngineWorker::EngineWorker(QObject *parent)
    : QObject(parent)
    , _keys()
{
    LOG;
}

EngineWorker::~EngineWorker()
{
    LOG;
}

void EngineWorker::setKeys(const QList<Qt::Key>& value)
{
    QMutexLocker locker(&this->_mutex);
    Q_UNUSED(locker);
    this->_keys = value;
}

QRegion getAllocatedRegion(const QList<Tile>& tiles, const Tile& current)
{
    QRegion result;
    for (const auto& tile: tiles) {
        if (!tile.isTheSame(current)) {
            if (tile.geometry().mask.rectCount()) {
                result += tile.geometry().mask;
            } else if (tile.visible && !tile.isFlag()) {
                result += tile.geometry().rect;
            }
        }
    }
    return result;
}

QList<Direction> getAvailableDirections(const QList<Tile>& tiles, const Tile& current, const QList<Direction>& directions)
{
    const QRegion allocatedRegion = getAllocatedRegion(tiles, current);
    QList<Direction> result;
    for (const auto direction: directions) {
        switch (direction) {
        case Direction::Up:
            if (!allocatedRegion.intersects(current.geometry().rect.translated(0, -1))) {
                result.append(direction);
            }
            break;
        case Direction::Right:
            if (!allocatedRegion.intersects(current.geometry().rect.translated(1, 0))) {
                result.append(direction);
            }
            break;
        case Direction::Down:
            if (!allocatedRegion.intersects(current.geometry().rect.translated(0, 1))) {
                result.append(direction);
            }
            break;
        case Direction::Left:
            if (!allocatedRegion.intersects(current.geometry().rect.translated(-1, 0))) {
                result.append(direction);
            }
            break;
        default:
            break;
        }
    }
    return result;
}

QList<Direction> getTragetDirections(const QList<Tile>& tiles, const Tile& tile)
{
    static QList<Direction> clockwise({Direction::Up, Direction::Right, Direction::Down, Direction::Left});

    const auto availableDirections = getAvailableDirections(tiles, tile, clockwise);
    if (!availableDirections.isEmpty()) {
        if (!availableDirections.contains(tile.direction)) {
            //blocked -> choose new one direction
            return QList<Direction>({availableDirections.at(random(availableDirections.count()))});
        } else if (tile.onFrame()) {
            if (!random(12)) {
                return QList<Direction>({availableDirections.at(random(availableDirections.count()))});
            }
        }
    }
    return QList<Direction>({tile.direction});
}

QList<Direction> getTragetDirectionsByKeys(const QList<Qt::Key>& keys)
{
    QList<Direction> result;
    for (const auto key: keys) {
        switch (key) {
        case Qt::Key_Up:
            result.append(Direction::Up);
            break;
        case Qt::Key_Right:
            result.append(Direction::Right);
            break;
        case Qt::Key_Down:
            result.append(Direction::Down);
            break;
        case Qt::Key_Left:
            result.append(Direction::Left);
            break;
        default:
            break;
        }
    }
    return result;
}

void updateShooting(QList<Tile>& tiles, Tile& curr)
{
    if (curr.isShooting()) {
        const bool isVertical =
                curr.shotDirection == Direction::Up ||
                curr.shotDirection == Direction::Down;
        const int x = curr.projectilePosition.x();
        const int y = curr.projectilePosition.y();
        const qreal hblock = isVertical ? BLOCK : BLOCK / 2; //TODO: check power and material
        const qreal vblock = isVertical ? BLOCK / 2 : BLOCK;
        /*
        const int left = (x - (isVertical ? BLOCK / 2 : 0));
        const int right = (x + (isVertical ? BLOCK / 2 : 0));
        const int top = (y - (isVertical ? 0 : BLOCK / 2));
        const int bottom = (y + (isVertical ? 0 : BLOCK / 2));
        */
        const int left = qFloor(1.0 * (x - (isVertical ? BLOCK / 2 : 0)) / hblock) * hblock;
        const int right = qCeil(1.0 * (x + (isVertical ? BLOCK / 2 : 0)) / hblock) * hblock;
        const int top = qFloor(1.0 * (y - (isVertical ? 0 : BLOCK / 2)) / vblock) * vblock;
        const int bottom = qCeil(1.0 * (y + (isVertical ? 0 : BLOCK / 2)) / vblock) * vblock;
        const int width = right - left;
        const int height = bottom - top;
        const QRect shootRect(left, top, width, height);
        for (Tile& tile: tiles) {
            if (tile.visible) {
                if (!tile.isTheSame(curr)) {
                    if (tile.geometry().mask.rectCount()) {
                        if (tile.geometry().mask.intersects(shootRect)) {
                            if (curr.canBrake(tile)) {
                                tile.brake(shootRect);
                            }
                            curr.shot();
                            return;
                        }
                    } else {
                        if (tile.geometry().rect.intersects(shootRect)) {
                            if (curr.canBrake(tile)) {
                                tile.brake();
                            }
                            curr.shot();
                        }
                    }
                }
            }
        }
    }
}

QPoint getRespawnPoint(const QList<Tile>& tiles, const Tile& tile)
{
    static const QPoint playerPoint(TILE * 5, TILE * 13);
    static const QPoint enemyPoint0(TILE, TILE);
    static const QPoint enemyPoint1(TILE * 7, TILE);
    static const QPoint enemyPoint2(TILE * 13, TILE);
    static const QList<QPoint> enemyPoints({enemyPoint0, enemyPoint1, enemyPoint2});

    const QPoint result = tile.isEnemy() ?
                enemyPoints.at(random(enemyPoints.count())) :
                playerPoint;
    if (!result.isNull()) {
        const QRegion allocated = getAllocatedRegion(tiles, tile);
        if (!allocated.intersects(QRect(result, tile.geometry().rect.size()))) {
            return result;
        }
    }
    return QPoint();
}

void EngineWorker::loop()
{
    QList<Tile> tiles = TilesLoader::load();
    QDateTime lastRespawn;
    QDateTime timePrev = QDateTime::currentDateTime();
    QHash<int, double> distances;
    QRegion fieldRegion;
    for (Tile& tile: tiles) {
        fieldRegion += tile._geometry.rect;
    }
    const QRect fieldRect = fieldRegion.boundingRect();

    forever {
        if (QThread::currentThread()->isInterruptionRequested() ) {
            break;
        }
        const auto timeCurr = QDateTime::currentDateTime();
        const auto duration = timePrev.msecsTo(timeCurr);

        QList<Qt::Key> keys;
        {
            QMutexLocker locker(&this->_mutex);
            Q_UNUSED(locker);
            keys = this->_keys;
        }
        for (Tile& tile: tiles) {
            distances[tile.id()] += (tile.speed * SCALE * 60.0 * duration / 1060.0);
            if (tile.isTank()) { // respawn and shooting
                if (!tile.isAlive()) {
                    const bool respawnEnemy =
                            tile.isEnemy() &&
                            (lastRespawn.isNull() || lastRespawn.msecsTo(timeCurr) > RESPAWN_INTERVAL) &&
                            Tile::aliveEnemiesCount(tiles) < 4;
                    if (tile.isPlayer() || respawnEnemy) {
                        const QPoint point = getRespawnPoint(tiles, tile);
                        if (!point.isNull()) {
                            tile.respawn(timeCurr, point, tile.isPlayer() ? Direction::Up : Direction::Down);
                            lastRespawn = tile.respawnTime;
                        }
                    }
                }
                tile.invulnerable = tile.isPlayer() && tile.respawnTime.msecsTo(timeCurr) < 3000;
                if (tile.isShooting()) {
                    const auto projectileSpeed = tile.isPlayer() ? (tile.level > 0 ? 4.0 : 2.0) : (tile.level == 2 ? 4.0 : 2.0);
                    const auto shootingDuration = tile.shotTime.msecsTo(timeCurr);
                    const auto shootingDistance = SCALE * projectileSpeed * 60.0 * shootingDuration / 1060.0;
                    const auto shootingOffset = toPoint(tile.shotDirection) * shootingDistance;
                    tile.projectilePosition = tile.shotPosition + shootingOffset;
                    updateShooting(tiles, tile);
                }
            }
            if (tile.isTank()) { // move player
                const auto targetDirections =
                        tile.isPlayer() ?
                            getTragetDirectionsByKeys(keys) :
                            getTragetDirections(tiles, tile);
                if (!targetDirections.isEmpty()) {
                    const auto availableDirections = getAvailableDirections(tiles, tile, targetDirections);
                    if (availableDirections.isEmpty()) {
                        tile.speed = 0.0;
                        if (tile.direction != targetDirections.first()) {
                            tile.direction = targetDirections.first();
                            distances[tile.id()] = 0.0;
                        }
                    } else { //if (!availableDirections.contains(tile.direction)) {
                        // available direction -> move move move
                        tile.speed = tile.isPlayer() ? (3.0 / 4.0) : (tile.level == 1 ? 1.0 : (2.0 / 4.0));
                        //tile.speed = 1.0;
                        if (tile.direction != availableDirections.first()) {
                            tile.direction = availableDirections.first();
                            distances[tile.id()] = 0.0;
                        } else {
                            if (distances[tile.id()] >= 1.0) {
                                distances[tile.id()] -= 1.0;
                                tile._geometry.rect.translate(toPoint(tile.direction));
                            }
                        }
                    }
                } else {
                    tile.speed = 0.0;
                    distances[tile.id()] = 0.0;
                }
            }
            if (tile.isTank()) { // shot
                const auto outOfSpace = !fieldRect.contains(tile.projectilePosition);
                if (tile.isPlayer() && keys.contains(Qt::Key_Space) && (tile.canShot(timeCurr) || outOfSpace)) {
                    tile.shot(timeCurr);
                } else if (tile.isEnemy() && tile.onFrame() && tile.canShot(timeCurr)) {
                    if (!random(12)) {
                        tile.shot(timeCurr);
                    }
                }
            }
        }
        emit this->updated(tiles);

        QCoreApplication::processEvents();
        QThread::currentThread()->usleep(50);
        timePrev = timeCurr;
    }
}
