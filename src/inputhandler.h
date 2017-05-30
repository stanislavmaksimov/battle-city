#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <QObject>
#include <QSet>
#include <QStack>
#include "common.h"

class InputHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<Qt::Key> keys READ keys NOTIFY keysChanged)

    QList<Qt::Key> _keys;

signals:
    void keysChanged(const QList<Qt::Key>& keys);

public:
    InputHandler(QObject *parent = nullptr);

    QList<Qt::Key> keys() const;

public slots:
    void reset();
    void press(int value, bool repeat);
    void release(int value, bool repeat);

private slots:
    void setKeys(const QList<Qt::Key>&);
};

#endif // INPUTHANDLER_H
