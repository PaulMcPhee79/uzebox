
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

#ifndef PROXYITEM_H
#define PROXYITEM_H

#include <QVariant>
#include <QString>
#include <PlatzGraphicsItem.h>
#include "WorldItem.h"

class ProxyItem : public WorldItem
{
public:
    ProxyItem();
    ProxyItem(const QList<QVariant> &data, WorldItem::WorldItemType type = WorldItem::Slice, WorldItem* parent = 0);
    ~ProxyItem();

    WorldItem* createItem(const QList<QVariant> &data, WorldItem *parent = 0);
    QVariant data(int column) const;
    QVariant dataDecoration(int column) const;
    QVariant tooltipData(int column) const;
    void setData(const QVariant &data);
    bool validChild(const WorldItem::WorldItemType &type) const;
    WorldItem::WorldItemType type() const;
    QRectF boundingRect() const;
    QRectF relativeBoundingRect() const;
    void setBoundingRect(const QRectF &r);
    void setRelativeBoundingRect(const QRectF &r);
    QRectF limitRect() const;
    qreal offsetX() const;
    //void setColumnData(int column, int count, const QVariant &data);

private:
    QString proxyData;
    WorldItem::WorldItemType proxyType;
};

#endif // PROXYITEM_H
