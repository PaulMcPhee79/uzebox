
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
#include "BgPlatformPath.h"

QString BgPlatformPath::platformPathFlagsToString(const int &flags)
{
    if (flags&AxisX)
        return "AXIS_X";
    else
        return "AXIS_Y";
}

BgPlatformPath::BgPlatformPath()
    : WorldItem(0), platformAxis(AxisX)
{
    platformPathData = "";
    WorldItem::worldStats.platformPathCount++;
}

BgPlatformPath::BgPlatformPath(const QList<QVariant> &data, WorldItem* parent)
    : WorldItem(parent), platformAxis(AxisX)
{
    if (data.length() > 0)
        platformPathData = data[0].toString();
    else
        platformPathData = "";
    WorldItem::worldStats.platformPathCount++;
}

WorldItem* BgPlatformPath::createItem(const QList<QVariant> &data, WorldItem *parent)
{
    BgPlatformPath *bgpp = new BgPlatformPath(data, (parent) ? parent : this->parent());

    if (bgpp) {
        bgpp->setAxis(platformAxis);
        bgpp->setRelativeBoundingRect(relativeBoundingRect());

        if (graphicalRepresentation())
            bgpp->setGraphicalRepresentation(new PlatzGraphicsItem(bgpp, graphicalRepresentation()->mode()));
    }
    return bgpp;
}

WorldItem* BgPlatformPath::validateState()
{
    if (childCount() != 1)
        return this;
    return 0;
}

QVariant BgPlatformPath::data(int column) const
{
    if (column == 0)
        return QVariant(platformPathData + " " + QString::number(row()));
    else
        return QVariant("");
}

QVariant BgPlatformPath::dataDecoration(int) const
{
    return QVariant(WorldItem::worldItemIcon(WorldItem::PlatformPathIcon));
}

QVariant BgPlatformPath::tooltipData(int column) const
{
    return data(column);
}

QString BgPlatformPath::detailData() const
{
    if (platformAxis == AxisX)
        return "Movement Axis: X Axis\nRange: " + QString::number((int)relativeBoundingRect().left()) +
                " to " + QString::number((int)relativeBoundingRect().right());
    else
        return "Movement Axis: Y Axis\nRange: " + QString::number((int)relativeBoundingRect().top()) +
                " to " + QString::number((int)relativeBoundingRect().bottom());
}

WorldItem::WorldItemType BgPlatformPath::type() const {
    return WorldItem::PlatformPath;
}

bool BgPlatformPath::validChild(const WorldItem::WorldItemType &type) const
{
    return type == WorldItem::Platform;
}

void BgPlatformPath::setData(const QVariant &data)
{
    platformPathData = data.toString();
}

qreal BgPlatformPath::offsetX() const
{
    WorldItem *p = this->parent();

    if (p)
        return p->offsetX(); // + p->boundingRect().left();
    else
        return 0.0;
}

void BgPlatformPath::setGraphicalRepresentation(PlatzGraphicsItem *item)
{
    WorldItem::setGraphicalRepresentation(item);

    if (item)
        item->setZValue(1.0);
}

BgPlatformPath::~BgPlatformPath()
{
    WorldItem::worldStats.platformPathCount--;
}
