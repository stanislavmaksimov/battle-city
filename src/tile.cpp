#include "tile.h"
#include "common.h"
#include "json.h"

int nextId()
{
    static QAtomicInteger<int> counter;
    return ++counter;
}

Tile::Tile(Type type, const Geometry& geometry)
    : _id(nextId())
    , _type(type)
    , _geometry(geometry)
    , direction()
    , speed(0.0)
    , level(0)
    , visible((type >= Brick && type <= Frame) || type >= Base)
    , invulnerable(false)
    , respawnTime()
    , shotTimePrev()
    , shotTime()
    , shotDirection()
    , shotPosition()
    , projectilePosition()
{
}

Tile::Tile(const Tile& other)
    : _id(other._id)
    , _type(other._type)
    , _geometry(other._geometry)
    , direction(other.direction)
    , speed(other.speed)
    , level(other.level)
    , visible(other.visible)
    , invulnerable(other.invulnerable)
    , respawnTime(other.respawnTime)
    , shotTimePrev(other.shotTimePrev)
    , shotTime(other.shotTime)
    , shotDirection(other.shotDirection)
    , shotPosition(other.shotPosition)
    , projectilePosition(other.projectilePosition)
{
}

bool Tile::canBrake(const Tile& other) const
{
    if (this->isTheSame(other)) {
        return false;
    }
    if (other.invulnerable) {
        return false;
    }
    if (other.isFlag()) {
        return false;
    }
    if (other.isFrame()) {
        return false;
    }
    if (other.isSteel() && level < 3) {
        return false;
    }
    if (this->isEnemy() && other.isEnemy()) {
        return false;
    }
    return true;
}
bool Tile::canShot(const QDateTime& currentTime) const
{
    return this->isAlive() && !this->isShooting() &&
            (this->shotTimePrev.isNull() || this->shotTimePrev.msecsTo(currentTime) > SHOT_INTERVAL);
}

void Tile::shot(const QDateTime& shotTime)
{
    if (shotTime.isValid() && !this->canShot(shotTime)) {
        return;
    }
    this->shotTimePrev = this->shotTime;
    this->shotTime = shotTime;
    this->shotDirection = this->direction;
    switch (this->shotDirection) {
    case Direction::Up:
        this->shotPosition = QPoint(this->_geometry.rect.center().x(), this->_geometry.rect.top() - 1);
        break;
    case Direction::Down:
        this->shotPosition = QPoint(this->_geometry.rect.center().x(), this->_geometry.rect.bottom() + 1);
        break;
    case Direction::Right:
        this->shotPosition = QPoint(this->_geometry.rect.right() + 1, this->_geometry.rect.center().y());
        break;
    case Direction::Left:
        this->shotPosition = QPoint(this->_geometry.rect.left() - 1, this->_geometry.rect.center().y());
        break;
    default:
        break;
    }
    if (!this->shotTime.isNull()) {
        this->projectilePosition = this->shotPosition;
    }
}

void Tile::brake(const QRect& rect)
{
    if (this->_geometry.mask.rectCount()) {
        this->_geometry.mask -= rect;
    } else if (this->isBase()) {
         this->_type = Flag;
    } else {
        this->visible = false;
    }
    this->shot();
}

void Tile::respawn(const QDateTime& currentTime, const QPoint& position, Direction direction)
{
    if (!this->isAlive()) {
        this->respawnTime = currentTime;
        this->_geometry.rect.moveTo(position);
        this->direction = direction;
        this->visible = true;
    }
}

bool Tile::isShooting() const
{
    return !this->shotTime.isNull();
}

bool Tile::onFrame() const
{
    return !(this->_geometry.rect.x() % BLOCKI) && !(this->_geometry.rect.y() % BLOCKI);
}

template<> QJsonValue toJsonValue<Tile::Type>(const Tile::Type& value)
{
    return QJsonValue(static_cast<int>(value));
}

template<> Tile::Type fromJsonValue<Tile::Type>(const QJsonValue& value)
{
    return static_cast<Tile::Type>(value.toInt());
}

template<> QJsonValue toJsonValue<Direction>(const Direction& value)
{
    return QJsonValue(static_cast<int>(value));
}

template<> Direction fromJsonValue<Direction>(const QJsonValue& value)
{
    return static_cast<Direction>(value.toInt());
}

template<> QJsonValue toJsonValue<Tile>(const Tile& value)
{
    QJsonObject result;
    result.insert("t", toJsonValue(value.type()));
    result.insert("s", toJsonValue(value.speed));
    result.insert("d", toJsonValue(value.direction));
    result.insert("l", toJsonValue(value.level));
    return result;
}

template<> Tile fromJsonValue<Tile>(const QJsonValue& value)
{
    if (value.isObject()) {
        const auto object = value.toObject();
        Tile result(fromJsonValue<Tile::Type>(object.value("t")));
        result.speed = fromJsonValue<qreal>(object.value("s"));
        result.direction = fromJsonValue<Direction>(object.value("d"));
        result.level = fromJsonValue<int>(object.value("l"));
        return result;
    }
    return Tile();
}

QString Tile::sprite() const
{
    static const QHash<Type, QString> sprites(
    {{Brick, SPRITE_RESOURCE1.arg(TILE_NAME_BRICK)},
     {Steel, SPRITE_RESOURCE1.arg(TILE_NAME_STEEL)},
     {Frame, SPRITE_RESOURCE1.arg(TILE_NAME_FRAME)},
     {Base, SPRITE_RESOURCE1.arg(TILE_NAME_BASE)},
     {Flag, SPRITE_RESOURCE1.arg(TILE_NAME_FLAG)}});

    if (this->isTank()) {
        return SPRITE_RESOURCE2.arg(TILE_NAME_TANK, toBase64String(*this));
    }
    return sprites.value(this->_type, QString::null);
}

QString Tile::mask() const
{
    if (!this->_geometry.mask.isEmpty()) {
        return SPRITE_RESOURCE2.arg(TILE_NAME_MASK, toBase64String(this->_geometry));
    }
    return QString::null;
}

int Tile::aliveEnemiesCount(const QList<Tile>& values)
{
    int result = 0;
    for (const auto& value: values) {
        if (value.isEnemy() && value.isAlive()) {
            ++result;
        }
    }
    return result;
}

bool operator !=(const Tile& l, const Tile& r)
{
    if (l._id != r._id)
        return true;
    if (l._type != r._type)
        return true;
    if (l._geometry != r._geometry)
        return true;
    if (l.direction != r.direction)
        return true;
    if (l.speed != r.speed)
        return true;
    if (l.visible != r.visible)
        return true;
    if (l.invulnerable != r.invulnerable)
        return true;
    if (l.respawnTime != r.respawnTime)
        return true;
    if (l.shotTimePrev != r.shotTimePrev)
        return true;
    if (l.shotTime != r.shotTime)
        return true;
    if (l.shotDirection != r.shotDirection)
        return true;
    if (l.shotPosition != r.shotPosition)
        return true;
    if (l.projectilePosition != r.projectilePosition)
        return true;
    return false;
}

bool operator ==(const Tile& l, const Tile& r)
{
    return !(l != r);
}
