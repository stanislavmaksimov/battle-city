#include "json.h"
#include <QRegion>
#include <QVector>

template<> QJsonValue toJsonValue<int>(const int& value)
{
    return QJsonValue(value);
}

template<> int fromJsonValue<int>(const QJsonValue& value)
{
    return value.toInt();
}

template<> QJsonValue toJsonValue<double>(const double& value)
{
    return QJsonValue(value);
}

template<> double fromJsonValue<double>(const QJsonValue& value)
{
    return value.toDouble();
}

template<> QJsonValue toJsonValue<QPoint>(const QPoint& value)
{
    QJsonObject result;
    result.insert("x", toJsonValue(value.x()));
    result.insert("y", toJsonValue(value.y()));
    return result;
}

template<> QPoint fromJsonValue<QPoint>(const QJsonValue& value)
{
    if (value.isObject()) {
        const auto object = value.toObject();
        return QPoint(fromJsonValue<int>(object.value("x")),
                      fromJsonValue<int>(object.value("y")));
    }
    return QPoint();
}

template<> QJsonValue toJsonValue<QPointF>(const QPointF& value)
{
    QJsonObject result;
    result.insert("x", toJsonValue(value.x()));
    result.insert("y", toJsonValue(value.y()));
    return result;
}

template<> QPointF fromJsonValue<QPointF>(const QJsonValue& value)
{
    if (value.isObject()) {
        const auto object = value.toObject();
        return QPointF(fromJsonValue<double>(object.value("x")),
                       fromJsonValue<double>(object.value("y")));
    }
    return QPointF();
}

template<> QJsonValue toJsonValue<QRect>(const QRect& value)
{
    QJsonObject result;
    result.insert("x", toJsonValue(value.x()));
    result.insert("y", toJsonValue(value.y()));
    result.insert("w", toJsonValue(value.width()));
    result.insert("h", toJsonValue(value.height()));
    return result;
}

template<> QRect fromJsonValue<QRect>(const QJsonValue& value)
{
    if (value.isObject()) {
        const auto object = value.toObject();
        return QRect(fromJsonValue<int>(object.value("x")),
                     fromJsonValue<int>(object.value("y")),
                     fromJsonValue<int>(object.value("w")),
                     fromJsonValue<int>(object.value("h")));
    }
    return QRect();
}

template<> QJsonValue toJsonValue<QRegion>(const QRegion& value)
{
    QJsonArray result;
    for (const auto& rect: value.rects()) {
        result.append(toJsonValue(rect));
    }
    return result;
}

template<> QRegion fromJsonValue<QRegion>(const QJsonValue& value)
{
    QRegion result;
    if (value.isArray()) {
        const auto array = value.toArray();
        for (const auto rect: array) {
            result |= fromJsonValue<QRect>(rect);
        }
    }
    return result;
}
