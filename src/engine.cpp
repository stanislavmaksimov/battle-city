#include "engine.h"
#include "common.h"
#include "tilesmodel.h"
#include "engineworker.h"
#include <QThread>

Engine::Engine(QObject* parent)
    : QObject(parent)
    , _size(FIELD_SIZE)
    , _thread(new QThread(this))
    , _model(new TilesModel(this))
    , _worker(new EngineWorker(nullptr))
{
    LOG;
    this->_worker->moveToThread(this->_thread);
    connect(this, SIGNAL(setKeys(QList<Qt::Key>)),
            this->_worker, SLOT(setKeys(QList<Qt::Key>)),
            Qt::QueuedConnection);
    connect(this->_thread, SIGNAL(started()),
            this->_worker, SLOT(loop()),
            Qt::QueuedConnection);
    connect(this->_worker, SIGNAL(updated(QList<Tile>)),
            qobject_cast<TilesModel*>(this->_model), SLOT(update(QList<Tile>)),
            Qt::QueuedConnection);
    this->_thread->start();
}

Engine::~Engine()
{
    this->_thread->requestInterruption();
    this->_thread->quit();
    this->_thread->wait();
    if (auto worker = this->_worker) {
        this->_worker = nullptr;
        delete worker;
    }
    LOG;
}
