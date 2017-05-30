#ifndef JSON_H
#define JSON_H

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QList>

template <typename T> QJsonValue toJsonValue(const T&);
template <typename T> T fromJsonValue(const QJsonValue& value);

template <typename T> QByteArray toJson(const T& value, bool compact = true)
{
    const QJsonValue jsonValue = toJsonValue(value);
    if (jsonValue.isArray()) {
        return QJsonDocument(jsonValue.toArray()).toJson(compact ? QJsonDocument::Compact : QJsonDocument::Indented);
    }
    return QJsonDocument(jsonValue.toObject()).toJson(compact ? QJsonDocument::Compact : QJsonDocument::Indented);
}

template <typename T> T fromJson(const QByteArray& value)
{
    QJsonDocument document = QJsonDocument::fromJson(value);
    if (document.isArray()) {
        return fromJsonValue<T>(document.array());
    }
    return fromJsonValue<T>(document.object());
}

template <typename T> QByteArray toBase64(const T& value)
{
    return toJson(value).toBase64();
}

template <typename T> QString toBase64String(const T& value)
{
    return QString::fromLatin1(toBase64(value));
}

template <typename T> T fromBase64(const QByteArray& value)
{
    return fromJson<T>(QByteArray::fromBase64(value));
}

template <typename T> T fromBase64(const QString& value)
{
    return fromBase64<T>(value.toLatin1());
}

#endif // JSON_H
