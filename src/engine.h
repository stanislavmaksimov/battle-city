#ifndef ENGINE_H
#define ENGINE_H

#include <QAbstractItemModel>
#include "common.h"

class QThread;
class EngineWorker;

class Engine : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QSize size MEMBER _size CONSTANT)
    Q_PROPERTY(QAbstractItemModel* model MEMBER _model CONSTANT)

    const QSize _size;
    QThread* const _thread;
    QAbstractItemModel* const _model;
    EngineWorker* _worker;

public:
    Engine(QObject* parent = nullptr);
    ~Engine();

signals:
    void setKeys(const QList<Qt::Key>&);
};

#endif // ENGINE_H
