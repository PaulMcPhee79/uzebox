
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

#ifndef PLATZGRAPHICSITEM_H
#define PLATZGRAPHICSITEM_H

#include <QGraphicsItem>
#include <PlatzEnums.h>

class WorldItem;

class PlatzGraphicsItem : public QGraphicsItem
{
public:
    PlatzGraphicsItem();
    PlatzGraphicsItem(WorldItem *parent = 0, Platz::ItemDisplayMode mode = Platz::NORMAL);
    PlatzGraphicsItem(PlatzGraphicsItem *item);
    PlatzGraphicsItem& operator=(const PlatzGraphicsItem &rhs);
    void platzPrepareGeometryChange();

    WorldItem* parent() const;
    void setParent(WorldItem *p) { itemParent = p; }
    QRectF boundingRect() const;
    QRectF relativeBoundingRect() const;
    void setBoundingRect(const QRectF &r);
    void setRelativeBoundingRect(const QRectF &r);
    void setOffsetX(qreal offset);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF limitRect() const;
    Platz::ItemDisplayMode mode() const;
    void setMode(Platz::ItemDisplayMode mode);
private:
    WorldItem *itemParent;
    QRectF bounds;  // For orphaned items (like the selection box)
    qreal offsetX;  // For orphaned items
    Platz::ItemDisplayMode displayMode;
};

#endif // PLATZGRAPHICSITEM_H
