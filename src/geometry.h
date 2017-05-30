#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QRegion>

class Geometry
{
public:
    QRect rect;
    QRegion mask;

    Geometry(const QRect& rect = QRect(), const QRegion& mask = QRegion());
    Geometry(const Geometry&);
};

bool operator !=(const Geometry& l, const Geometry& r);
bool operator ==(const Geometry& l, const Geometry& r);

#endif // GEOMETRY_H
