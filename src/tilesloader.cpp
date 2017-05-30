#include "tilesloader.h"
#include "tile.h"

QList<Tile> TilesLoader::load()
{
    QList<Tile> tiles;
    {
        QRegion region;
        region += QRect(TILE * 2, TILE * 2, TILE, TILE * 4.5);
        region += QRect(TILE * 4, TILE * 2, TILE, TILE * 4.5);
        region += QRect(TILE * 6, TILE * 2, TILE, TILE * 3.5);
        region += QRect(TILE * 8, TILE * 2, TILE, TILE * 3.5);
        region += QRect(TILE * 10, TILE * 2, TILE, TILE * 4.5);
        region += QRect(TILE * 12, TILE * 2, TILE, TILE * 4.5);
        region += QRect(TILE * 6, TILE * 6.5, TILE, TILE);
        region += QRect(TILE * 8, TILE * 6.5, TILE, TILE);
        region += QRect(TILE, TILE * 7.5, TILE, TILE * 0.5);
        region += QRect(TILE * 13, TILE * 7.5, TILE, TILE * 0.5);
        region += QRect(TILE * 3, TILE * 7.5, TILE * 2, TILE);
        region += QRect(TILE * 10, TILE * 7.5, TILE * 2, TILE);
        region += QRect(TILE * 6, TILE * 8.5, TILE, TILE * 3);
        region += QRect(TILE * 8, TILE * 8.5, TILE, TILE * 3);
        region += QRect(TILE * 7, TILE * 9, TILE, TILE);
        region += QRect(TILE * 2, TILE * 9.5, TILE, TILE * 3.5);
        region += QRect(TILE * 4, TILE * 9.5, TILE, TILE * 3.5);
        region += QRect(TILE * 10, TILE * 9.5, TILE, TILE * 3.5);
        region += QRect(TILE * 12, TILE * 9.5, TILE, TILE * 3.5);
        region += QRect(TILE * 6.5, TILE * 12.5, TILE * 2, TILE * 1.5);
        region -= QRect(TILE * 7, TILE * 13, TILE, TILE);
        tiles << Tile(Tile::Brick, Geometry(QRect(QPoint(), FIELD_SIZE), region));
    }
    {
        QRegion region;
        region += QRect(QPoint(), FIELD_SIZE);
        region -= QRect(TILE, TILE, TILE * 13, TILE * 13);
        tiles << Tile(Tile::Frame, Geometry(QRect(QPoint(), FIELD_SIZE), region));
    }
    {
        QRegion region;
        region += QRect(TILE, TILE * 8, TILE, TILE * 0.5);
        region += QRect(TILE * 13, TILE * 8, TILE, TILE * 0.5);
        region += QRect(TILE * 7, TILE * 4, TILE, TILE);
        tiles << Tile(Tile::Steel, Geometry(QRect(QPoint(), FIELD_SIZE), region));
    }
    {
        tiles << Tile(Tile::Base, QRect(TILE * 7, TILE * 13, TILE, TILE));
    }
    {
        tiles << Tile(Tile::Tank, QRect(TILE * 5, TILE * 13, TILE, TILE));
    }
    {
        for (int i = 0; i < 4; ++i) {
            Tile tank(Tile::Enemy, QRect(TILE * 7, TILE, TILE, TILE));
            tank.level = i;
            tiles << tank;
        }
    }
    return tiles;
}
