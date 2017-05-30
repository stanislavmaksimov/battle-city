#include "inputhandler.h"

InputHandler::InputHandler(QObject *parent)
    : QObject(parent)
    , _keys()
{
}

QList<Qt::Key> InputHandler::keys() const
{
    return this->_keys;
}

void InputHandler::reset()
{
    this->setKeys(QList<Qt::Key>());
}

void InputHandler::press(int value, bool)
{
    const auto key = static_cast<Qt::Key>(value);
    auto keys = this->_keys;
    if (keys.contains(key)) {
        keys.removeOne(key);
    }
    keys.push_front(key);
    this->setKeys(keys);
}

void InputHandler::release(int value, bool repeat)
{
    if (repeat) return;

    auto keys = this->_keys;
    keys.removeOne(static_cast<Qt::Key>(value));
    this->setKeys(keys);
}

void InputHandler::setKeys(const QList<Qt::Key>& value)
{
    if (this->_keys != value) {
        this->_keys = value;
        emit this->keysChanged(this->_keys);
    }
}
