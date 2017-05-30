#ifndef TILESLOADER_H
#define TILESLOADER_H

#include <QList>

class Tile;

class TilesLoader
{
public:
    static QList<Tile> load();
};

#endif // TILESLOADER_H
