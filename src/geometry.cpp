#include "geometry.h"
#include "json.h"

Geometry::Geometry(const QRect& rect, const QRegion& mask)
    : rect(rect)
    , mask(mask)
{
}

Geometry::Geometry(const Geometry& other)
    : rect(other.rect)
    , mask(other.mask)
{
}

bool operator !=(const Geometry& l, const Geometry& r)
{
    if (l.rect != r.rect)
        return true;
    if (l.mask != r.mask)
        return true;
    return false;
}

bool operator ==(const Geometry& l, const Geometry& r)
{
    return !(l != r);
}

template<> QJsonValue toJsonValue<Geometry>(const Geometry& value)
{
    QJsonObject result;
    result.insert("r", toJsonValue(value.rect));
    result.insert("m", toJsonValue(value.mask));
    return result;
}

template<> Geometry fromJsonValue<Geometry>(const QJsonValue& value)
{
    if (value.isObject()) {
        const auto object = value.toObject();
        return Geometry({fromJsonValue<QRect>(object.value("r")),
                         fromJsonValue<QRegion>(object.value("m"))});
    }
    return Geometry();
}

