
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

#include "WorldItem.h"
#include "BgObject.h"

BgObject::BgObject()
    : WorldItem(0), bgMap(0)
{
    objectData = "";
    WorldItem::worldStats.objectCount++;
}

BgObject::BgObject(const QList<QVariant> &data, WorldItem* parent)
    : WorldItem(parent), bgMap(-1)
{
    if (data.length() > 0)
        objectData = data[0].toString();
    else
        objectData = "";
    WorldItem::worldStats.objectCount++;
}

WorldItem* BgObject::createItem(const QList<QVariant> &data, WorldItem *parent)
{
    BgObject *obj = new BgObject(data, (parent) ? parent : this->parent());

    if (obj) {
        obj->setMap(bgMap);
        obj->setRelativeBoundingRect(relativeBoundingRect());

        if (graphicalRepresentation())
            obj->setGraphicalRepresentation(new PlatzGraphicsItem(obj, graphicalRepresentation()->mode()));
    }
    return obj;
}

QVariant BgObject::data(int column) const
{
    if (column == 0)
        return QVariant(objectData + " " + QString::number(row()));
    else
        return QVariant("");
}

QVariant BgObject::dataDecoration(int) const
{
    return QVariant(WorldItem::worldItemIcon(WorldItem::ObjectIcon));
}

QVariant BgObject::tooltipData(int column) const
{
    return data(column);
}

QString BgObject::detailData() const
{
    return "Map Index: " + QString::number(bgMap);
}

WorldItem::WorldItemType BgObject::type() const {
    return WorldItem::Object;
}

bool BgObject::validChild(const WorldItem::WorldItemType&) const
{
    return false;
}

void BgObject::setData(const QVariant &data)
{
    objectData = data.toString();
}

qreal BgObject::offsetX() const
{
    WorldItem *p = this->parent();

    if (p)
        return p->offsetX();
    else
        return 0.0;
}

BgObject::~BgObject()
{
    WorldItem::worldStats.objectCount--;
}
