#ifndef TILESMODEL_H
#define TILESMODEL_H

#include <QAbstractListModel>
#include <QMutex>
#include "tile.h"

class TilesModel : public QAbstractListModel
{
    Q_OBJECT

    QMutex _mutex;
    QList<Tile> _data;

public:
    enum Roles
    {
        RectRole = Qt::UserRole + 1,
        MaskRole,
        SpriteRole,
        VisibleRole,
        ProjectileVisibleRole,
        ProjectilePositionRole,
        InvulnerableRole
    };

    TilesModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

public slots:
    void update(const QList<Tile>&);
};

#endif // TILESMODEL_H
