#include "tilesmodel.h"
#include "common.h"

TilesModel::TilesModel(QObject *parent)
    : QAbstractListModel(parent)
    , _mutex()
    , _data()
{
}

QHash<int, QByteArray> TilesModel::roleNames() const
{
    auto result = QAbstractListModel::roleNames();
    result[RectRole] = "rect_role";
    result[MaskRole] = "mask_role";
    result[SpriteRole] = "sprite_role";
    result[VisibleRole] = "visible_role";
    result[ProjectileVisibleRole] = "projectile_visible_role";
    result[ProjectilePositionRole] = "projectile_position_role";
    result[InvulnerableRole] = "invulnerable_role";
    return result;
}

int TilesModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return this->_data.count();
    }
    return 0;
}

QVariant TilesModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && index.row() >= 0 && index.row() < this->_data.count()) {
        if (role >= Qt::UserRole) {
            const Tile& tile = this->_data.at(index.row());
            switch (role) {
            case RectRole:
                return tile.geometry().rect;
            case MaskRole:
                return tile.mask();
            case SpriteRole:
                return tile.sprite();
            case VisibleRole:
                return tile.visible;
            case ProjectileVisibleRole:
                return tile.isShooting();
            case ProjectilePositionRole:
                return tile.projectilePosition;
            case InvulnerableRole:
                return tile.invulnerable;
            default:
                break;
            }
        }
    }
    return QVariant();
}

void TilesModel::update(const QList<Tile>& values)
{
    QMutexLocker locker(&this->_mutex);
    Q_UNUSED(locker);

    if (values.isEmpty()) {
        this->beginResetModel();
        this->_data.clear();
        this->endResetModel();
        return;
    }

    const auto countPrev = this->_data.count();
    const auto countCurr = values.count();

    if (countCurr < countPrev) {
        this->beginRemoveRows(QModelIndex(), countCurr, countPrev);
        while (this->_data.count() > countCurr) {
            this->_data.removeLast();
        }
        this->endRemoveRows();

    } else if (countCurr > countPrev) {
        this->beginInsertRows(QModelIndex(), countPrev, countCurr - 1);
        for (int i = countPrev; i < countCurr; ++i) {
            this->_data.append(values.at(i));
        }
        this->endInsertRows();

    }
    for (int i = 0, count = qMin(countPrev, countCurr); i < count; ++i) {
        if (this->_data.at(i) != values.at(i)) {
            this->_data.replace(i, values.at(i));
            const QModelIndex index = this->index(i);
            //TODO: check changed roles
            emit this->dataChanged(index, index);
        }
    }
}
