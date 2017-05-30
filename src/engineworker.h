#ifndef ENGINEWORKER_H
#define ENGINEWORKER_H

#include <QMutex>
#include <QObject>
#include "common.h"

class Tile;

class EngineWorker : public QObject
{
    Q_OBJECT

    QMutex _mutex;
    QList<Qt::Key> _keys;

signals:
    void updated(const QList<Tile>&);

public:
    explicit EngineWorker(QObject *parent = 0);
    virtual ~EngineWorker();

public slots:
    void loop();
    void setKeys(const QList<Qt::Key>& value);
};

#endif // ENGINEWORKER_H
