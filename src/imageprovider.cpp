#include "imageprovider.h"
#include "geometry.h"
#include "common.h"
#include "json.h"
#include "tile.h"
#include <QImageReader>
#include <QPainter>
#include <QBitmap>
#include <QPixmap>

QPixmap readPixmap(const QString& fileName, const QRect& sourceRect, const QSize& requestedSize, double scale)
{
    static const QColor backgroundColor(0, 0, 1, 255);

    QImageReader reader(fileName);
    if (reader.canRead()) {;
        if (sourceRect.isValid()) {
            reader.setClipRect(sourceRect);
        }
        QPixmap result = QPixmap::fromImageReader(&reader);
        result.setMask(result.createMaskFromColor(backgroundColor, Qt::MaskInColor));
        return result.scaled(requestedSize.isValid() ? requestedSize : sourceRect.size() * scale);
    }
    return QPixmap();
}

ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *pixmapSize, const QSize &requestedSize)
{
    static const QPoint offsetEnemy(8 * TILE_ORIGINAL, 4 * TILE_ORIGINAL);
    static const QPoint offsetPlayer(0, 0);
    static const QPoint offsetBrick(16 * TILE_ORIGINAL, 4 * TILE_ORIGINAL);
    static const QPoint offsetSteel(16 * TILE_ORIGINAL, 4 * TILE_ORIGINAL + BLOCK_ORIGINAL);
    static const QPoint offsetFrame(24 * TILE_ORIGINAL, 0);
    static const QPoint offsetBase(19 * TILE_ORIGINAL, 2 * TILE_ORIGINAL);
    static const QPoint offsetFlag(20 * TILE_ORIGINAL, 2 * TILE_ORIGINAL);

    static const QRect rectBrick(offsetBrick, BLOCK_SIZE_ORIGINAL);
    static const QRect rectSteel(offsetSteel, BLOCK_SIZE_ORIGINAL);
    static const QRect rectFrame(offsetFrame, BLOCK_SIZE_ORIGINAL);
    static const QRect rectBase(offsetBase, TILE_SIZE_ORIGINAL);
    static const QRect rectFlag(offsetFlag, TILE_SIZE_ORIGINAL);

    const QStringList parts = id.split("/");
    if (!parts.isEmpty()) {
        const QString& first = parts.first();

        if (first == TILE_NAME_BRICK) {
            if (pixmapSize) {
                *pixmapSize = BLOCK_SIZE;
            }
            return readPixmap(SPRITES, rectBrick, requestedSize, SCALE);

        } else if (first == TILE_NAME_STEEL) {
            if (pixmapSize) {
                *pixmapSize = BLOCK_SIZE;
            }
            return readPixmap(SPRITES, rectSteel, requestedSize, SCALE);

        } else if (first == TILE_NAME_FRAME) {
            if (pixmapSize) {
                *pixmapSize = BLOCK_SIZE;
            }
            return readPixmap(SPRITES, rectFrame, requestedSize, SCALE);

        } else if (first == TILE_NAME_BASE) {
            if (pixmapSize) {
                *pixmapSize = TILE_SIZE;
            }
            return readPixmap(SPRITES, rectBase, requestedSize, SCALE);

        } else if (first == TILE_NAME_FLAG) {
            if (pixmapSize) {
                *pixmapSize = TILE_SIZE;
            }
            return readPixmap(SPRITES, rectFlag, requestedSize, SCALE);

        } else if (parts.first() == TILE_NAME_INVULNERABLE) {
            if (pixmapSize) {
                *pixmapSize = TILE_SIZE;
            }
            const int index = parts.count() > 1 ? parts.at(1).toInt() : 0;
            const QPoint offset((16 + index) * TILE_ORIGINAL, 9 * TILE_ORIGINAL);
            return readPixmap(SPRITES, QRect(offset, TILE_SIZE_ORIGINAL), requestedSize, SCALE);

        } else if (first == TILE_NAME_TANK) {
            if (pixmapSize) {
                *pixmapSize = TILE_SIZE;
            }
            const auto tile = fromBase64<Tile>(parts.count() > 1 ? parts.at(1) : "");
            const QPoint offsetType = tile.isEnemy() ? offsetEnemy : offsetPlayer;
            const QPoint offsetDirection(2 * static_cast<int>(tile.direction) * TILE_ORIGINAL, 0);
            const QPoint offsetLevel(0, tile.level * TILE_ORIGINAL);
            const QPoint offset = offsetType + offsetDirection + offsetLevel;
            const QRect rectTank(offset, TILE_SIZE_ORIGINAL);
            return readPixmap(SPRITES, rectTank, requestedSize, SCALE);

        } else if (parts.first() == TILE_NAME_MASK) {
            const auto geometry = fromBase64<Geometry>(parts.count() > 1 ? parts.at(1) : "");
            if (pixmapSize) {
                *pixmapSize = geometry.rect.size();
            }
            QPixmap pixmap(geometry.rect.size());
            pixmap.fill(QColor(Qt::transparent));
            QPainter painter(&pixmap);
            painter.setClipRegion(geometry.mask);
            painter.fillRect(QRect(QPoint(), geometry.rect.size()), Qt::SolidPattern);
            if (requestedSize.isValid()) {
                return pixmap.scaled(requestedSize);
            }
            return pixmap;
        }
    }
    return QPixmap();
}
