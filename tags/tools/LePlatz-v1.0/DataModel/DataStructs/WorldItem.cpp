
/*
 *  LePlatz - A level editor for the Platz toolset (Uzebox - supports VIDEO_MODE 3)
 *  Copyright (C) 2009 Paul McPhee
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include "WorldItem.h"

WorldItem::WorldStats WorldItem::worldStats;

QList<QIcon> WorldItem::WorldItemIcons;
QList<QPixmap> WorldItem::WorldDetailPixmaps;
QStringList WorldItem::mutableIds;
QStringList WorldItem::triggerIds;
QStringList WorldItem::platClrTileIds;

void WorldItem::loadWorldItemIcons()
{
    WorldItemIcons.clear();

    for (int i = 0; i <= (int)WorldItem::QueryIcon; i++)
        WorldItemIcons.append(QIcon(":/icons/16x16/" + QString::number(i) + ".png"));
    WorldItemIcons.append(QIcon()); // Empty Icon for invalid indices
}

void WorldItem::loadWorldDetailPixmaps()
{
    WorldDetailPixmaps.clear();

    WorldDetailPixmaps.append(QPixmap(":/icons/128x128/slice.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    WorldDetailPixmaps.append(QPixmap(":/icons/128x128/bgo.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    WorldDetailPixmaps.append(QPixmap(":/icons/128x128/bgi.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    WorldDetailPixmaps.append(QPixmap(":/icons/128x128/object.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    WorldDetailPixmaps.append(QPixmap(":/icons/128x128/mpPath.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    WorldDetailPixmaps.append(QPixmap(":/icons/128x128/mp.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    WorldDetailPixmaps.append(QPixmap(":/icons/128x128/bgm.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    WorldDetailPixmaps.append(QPixmap()); // Empty Pixmap for invalid indices
/*
    for (int i = 0; i <= 6; i++)
        WorldDetailPixmaps.append(QPixmap(":/details/" + QString::number(i) + ".png"));
    WorldDetailPixmaps.append(QPixmap()); // Empty Pixmap for invalid indices
*/
}

const QIcon& WorldItem::worldItemIcon(const WorldItemIconIndex &index)
{
    if (WorldItemIcons.count() > index)
        return WorldItemIcons.at(index);
    else
        return WorldItemIcons.last();
}

const QPixmap& WorldItem::worldDetailPixmap(const WorldItemType &type)
{
    switch (type) {
        case Slice: return WorldDetailPixmaps.at(0); break;
        case Outer: return WorldDetailPixmaps.at(1); break;
        case Inner: return WorldDetailPixmaps.at(2); break;
        case Object:  return WorldDetailPixmaps.at(3); break;
        case PlatformPath:  return WorldDetailPixmaps.at(4); break;
        case Platform:  return WorldDetailPixmaps.at(5); break;
        case Mutable:  return WorldDetailPixmaps.at(6); break;
        default:  return WorldDetailPixmaps.last(); break;
    }
}

WorldItem::WorldItem()
    : graphicsItem(0), parentItem(0), mutableBgCount(0)
{
}

WorldItem::WorldItem(WorldItem *parent)
    : graphicsItem(0), parentItem(parent), mutableBgCount(0)
{
}

void WorldItem::appendChild(WorldItem *child)
{
    childItems.append(child);
    child->setParent(this);
}

QString WorldItem::detailData() const
{
    return "";
}

void WorldItem::insertChild(int pos, WorldItem *child)
{
    if (pos < childItems.count())
        childItems.insert(pos, child);
    else
        appendChild(child);
    child->setParent(this);
}

WorldItem* WorldItem::child(int row) const
{
    if (row >= 0 && row < childItems.count())
        return childItems.value(row);
    else
        return 0;
}

const QList<WorldItem*>* WorldItem::children(void)
{
    return &childItems;
}

int WorldItem::childCount() const
{
    return childItems.count();
}

int WorldItem::columnCount() const
{
    return 1;
}

int WorldItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<WorldItem*>(this));

    return -1;
}

WorldItem* WorldItem::parent() const
{
    return parentItem;
}

void WorldItem::setParent(WorldItem *parent)
{
    parentItem = parent;
}

void WorldItem::setChildRowData(int, int, const QVariant&)
{
    /*
    const QList<QVariant> &dataList = reinterpret_cast<const QList<QVariant>&>(data);

    foreach (const QVariant &v, dataList) {
        if (row < childCount())
            child(row++)->setData(v);
        else
            break;
    }
    */
}

void WorldItem::setChildColumnData(int, int, int, const QVariant&)
{
    // Let child handle validity checks
    //child(row)->setColumnData(column, count, data);
}

bool WorldItem::removeChild(WorldItem *child)
{
    return childItems.removeOne(child);
}

bool WorldItem::removeChildRows(int row, int count)
{
    for (int i = row; i < (row+count); i++) {
        if (row < childCount()) {
            WorldItem* del = childItems.at(row);

            if (del && del->removeChildRows(0, del->childCount())) {
                if (childItems.removeOne(del)) {
                    if (del)
                        delete del;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }
    }
    return true;
}

void WorldItem::incMutableCount() {
    ++mutableBgCount;

    if(parent())
        parent()->incMutableCount();
}

void WorldItem::decMutableCount() {
    --mutableBgCount;

    if (parent())
        parent()->decMutableCount();
}

WorldItem* WorldItem::validateState()
{
    if (!parent())
        return this;
    foreach (WorldItem *child, *children()) {
        WorldItem *w = child->validateState();
        if (w)
            return w;
    }
    return 0;
}

int WorldItem::mutableCount(int index) const
{
    if (index == -1)
        return mutableBgCount;    // Recursive calls end here

    int count = 0;
    index = qMin(index, childCount()-1);

    for (int i = index-1; i >= 0; i--)
        count += child(i)->mutableCount();
    if (parent())
        count += parent()->mutableCount(row());
    return count;
}

QRectF WorldItem::boundingRect() const
{
    return bounds.adjusted(offsetX(), 0, offsetX(), 0);
}

QRectF WorldItem::relativeBoundingRect() const
{
    return bounds;
}

void WorldItem::setBoundingRect(const QRectF &r)
{
    QRectF limits(limitRect()), newBounds(r);

    if ((type() != WorldItem::Slice) && !limits.contains(newBounds)) {
        newBounds.setLeft(qMax(limits.left(), newBounds.left()));
        newBounds.setTop(qMax(limits.top(), newBounds.top()));
        newBounds.setRight(qMin(limits.right(), newBounds.right()));
        newBounds.setBottom(qMin(limits.bottom(), newBounds.bottom()));
    }

    if (!graphicsItem) {
        bounds = newBounds.adjusted(-offsetX(), 0.0, -offsetX(), 0.0);
    } else {
        graphicsItem->platzPrepareGeometryChange();
        bounds = newBounds.adjusted(-offsetX(), 0.0, -offsetX(), 0.0);
        graphicsItem->update(); // Should be done automatically if required (apparently). We make sure.
    }
}

void WorldItem::setRelativeBoundingRect(const QRectF &r)
{
    setBoundingRect(r.adjusted(offsetX(), 0, offsetX(), 0));
}

QRectF WorldItem::limitRect() const
{
    if (parentItem)
        return parentItem->boundingRect();
    else
        return QRectF(0.0, 0.0, 0.0, 0.0);
}

void WorldItem::setDisplayMode(Platz::ItemDisplayMode mode)
{
    graphicsItem->setMode(mode);
}

PlatzGraphicsItem* WorldItem::graphicalRepresentation()
{
    return graphicsItem;
}

void WorldItem::setGraphicalRepresentation(PlatzGraphicsItem *item)
{
    graphicsItem = item;
}

WorldItem::~WorldItem()
{
    delete graphicsItem;
}
