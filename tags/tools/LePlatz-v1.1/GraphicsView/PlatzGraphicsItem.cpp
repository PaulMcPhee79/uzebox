
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

#include <QPainter>
#include <QStyleOption>
#include <WorldItem.h>
#include <PlatzGraphicsScene.h>
#include "PlatzGraphicsItem.h"

PlatzGraphicsItem::PlatzGraphicsItem()
    : itemParent(0), bounds(QRectF(0.0, 0.0, 0.0, 0.0)), offsetX(0.0), displayMode(Platz::NORMAL)
{
}

PlatzGraphicsItem::PlatzGraphicsItem(WorldItem *parent, Platz::ItemDisplayMode mode)
    : itemParent(parent), displayMode(mode)
{
}

PlatzGraphicsItem::PlatzGraphicsItem(PlatzGraphicsItem *item)
    : itemParent(0), bounds(QRectF(0.0, 0.0, 0.0, 0.0)), offsetX(0.0), displayMode(Platz::NORMAL)
{
    *this = *item;
}

PlatzGraphicsItem& PlatzGraphicsItem::operator=(const PlatzGraphicsItem &rhs)
{
    itemParent = const_cast<PlatzGraphicsItem&>(rhs).parent();
    displayMode = rhs.mode();
    return *this;
}

WorldItem* PlatzGraphicsItem::parent() const
{
    return itemParent;
}

QRectF PlatzGraphicsItem::relativeBoundingRect() const
{
    if (parent())
        return parent()->relativeBoundingRect();
    else
        return bounds;
}

QRectF PlatzGraphicsItem::boundingRect() const
{
    if (parent())
        return parent()->boundingRect();
    else
        return bounds.adjusted(offsetX, 0.0, offsetX, 0.0);  // For orphaned items (like selection box)
}

void PlatzGraphicsItem::platzPrepareGeometryChange()
{
    prepareGeometryChange();
}

void PlatzGraphicsItem::setBoundingRect(const QRectF &r)
{
    if (parent()) {
        parent()->setBoundingRect(r);
    } else {
        prepareGeometryChange();
        bounds = r.adjusted(-offsetX, 0, -offsetX, 0);
    }
}

void PlatzGraphicsItem::setRelativeBoundingRect(const QRectF &r)
{
    if (parent())
        setBoundingRect(r.adjusted(parent()->offsetX(), 0, parent()->offsetX(), 0));
    else
        bounds = r;
}

QRectF PlatzGraphicsItem::limitRect() const
{
    if (parent())
        return parent()->limitRect();
    else
        return QRectF(0.0, 0.0, 0.0, 0.0);
}

void PlatzGraphicsItem::setOffsetX(qreal offset)
{
    offsetX = offset;
}

void PlatzGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *) {
    int index;

    switch (displayMode) {
        case Platz::NORMAL:
            if (!parent())
                return;
            switch (parent()->type()) {
                case WorldItem::Outer:
                    index = 0;
                    break;
                case WorldItem::Inner:
                    index = 1;
                    break;
                case WorldItem::Mutable:
                    index = 2;
                    break;
                case WorldItem::Object:
                    index = 3;
                    break;
                case WorldItem::PlatformPath:
                    index = 4;
                    break;
                case WorldItem::Platform:
                    index = 5;
                    break;
                default:
                    index = -1;
                    return;
            }
            break;
        case Platz::SELECTED:
            index = 6;
            break;
        default:
            return;
    }

    painter->setPen(PlatzGraphicsScene::pens.at(index));
    painter->setBrush(PlatzGraphicsScene::brushes.at(index));
    painter->drawRect(boundingRect());
}

Platz::ItemDisplayMode PlatzGraphicsItem::mode() const
{
    return displayMode;
}

void PlatzGraphicsItem::setMode(Platz::ItemDisplayMode mode) {
    displayMode = mode;
    update();
}

