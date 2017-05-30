#include "common.h"

int random(int max)
{
    static bool randomized = []() { qsrand(QDateTime::currentDateTime().toTime_t()); return true; }();
    Q_UNUSED(randomized);

    return qrand() % max;
}

uint qHash(const Direction& value, uint seed)
{
    return qHash(static_cast<int>(value), seed);
}

template<> QPoint toPoint<Direction>(const Direction& value)
{
    QHash<Direction, QPoint> points(
    {{Direction::Up, QPoint(0, -1)},
     {Direction::Right, QPoint(1, 0)},
     {Direction::Down, QPoint(0, 1)},
     {Direction::Left, QPoint(-1, 0)}});

    return points.value(value, QPoint());
}
