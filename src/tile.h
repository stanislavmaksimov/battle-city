#ifndef TILE_H
#define TILE_H

#include "common.h"
#include "geometry.h"

class Tile
{
public:
    enum Type
    {
        None,
        Tank,
        Brick,
        Steel,
        Frame,
        Enemy,
        Base,
        Flag
    };

private:
    int _id;
    Type _type;
    Geometry _geometry;

public:
    Direction direction;
    qreal speed;
    int level;
    bool visible;
    bool invulnerable;
    QDateTime respawnTime;
    QDateTime shotTimePrev;
    QDateTime shotTime;
    Direction shotDirection;
    QPoint shotPosition;
    QPoint projectilePosition;

    inline bool isTheSame(const Tile& other) const { return this->_id == other._id; }
    inline const Geometry& geometry() const { return this->_geometry; }
    inline int type() const { return this->_type; }
    inline int id() const { return this->_id; }
    inline bool isPlayer() const { return this->_type == Tank; }
    inline bool isEnemy() const { return this->_type == Enemy; }
    inline bool isTank() const { return this->isEnemy() || this->isPlayer(); }
    inline bool isBase() const { return this->_type == Base; }
    inline bool isFlag() const { return this->_type == Flag; }
    inline bool isFrame() const { return this->_type == Frame; }
    inline bool isSteel() const { return this->_type == Steel; }
    inline bool isAlive() const { return !this->respawnTime.isNull() && this->visible; }

    bool onFrame() const;

    bool isShooting() const;
    bool canShot(const QDateTime& currentTime) const;
    void shot(const QDateTime& shotTime = QDateTime());

    bool canBrake(const Tile&) const;
    void brake(const QRect& = QRect());

    void respawn(const QDateTime& currentTime, const QPoint& position, Direction direction);

    QString sprite() const;
    QString mask() const;

    Tile(Type type = None, const Geometry& geometry = Geometry());
    Tile(const Tile&);

    static int aliveEnemiesCount(const QList<Tile>&);

    friend bool operator !=(const Tile& l, const Tile& r);
    friend bool operator ==(const Tile& l, const Tile& r);

    friend class EngineWorker;
};


#endif // TILE_H
