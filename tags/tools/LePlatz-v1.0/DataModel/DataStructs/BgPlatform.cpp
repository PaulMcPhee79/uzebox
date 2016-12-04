
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
#include "BgPlatform.h"


QString BgPlatform::platformFlagsToString(const int &flags)
{
    if (flags&Smooth)
        return "MP_SMOOTH";
    else
        return "MP_STEPPED";
}

BgPlatform::BgPlatform()
    : WorldItem(0), clrTile(0), platformFlags(Smooth), vel(0)
{
    platformData = "";
    WorldItem::worldStats.platformCount++;
}

BgPlatform::BgPlatform(const QList<QVariant> &data, WorldItem* parent)
    : WorldItem(parent), clrTile(0), platformFlags(Smooth), vel(0)
{
    if (data.length() > 0)
        platformData = data[0].toString();
    else
        platformData = "";
    WorldItem::worldStats.platformCount++;
}

WorldItem* BgPlatform::createItem(const QList<QVariant> &data, WorldItem *parent)
{
    BgPlatform *bgp = new BgPlatform(data, (parent) ? parent : this->parent());

    if (bgp) {
        bgp->setClearTile(clrTile);
        bgp->setFlags(platformFlags);
        bgp->setVelocity(vel);
        bgp->setRelativeBoundingRect(relativeBoundingRect());

        if (graphicalRepresentation())
            bgp->setGraphicalRepresentation(new PlatzGraphicsItem(bgp, graphicalRepresentation()->mode()));
    }
    return bgp;
}

QVariant BgPlatform::data(int column) const
{
    if (column == 0)
        return QVariant(platformData + " " + QString::number(row()));
    else
        return QVariant("");
}

QVariant BgPlatform::dataDecoration(int) const
{
    return QVariant(WorldItem::worldItemIcon(WorldItem::PlatformIcon));
}

QVariant BgPlatform::tooltipData(int column) const
{
    return data(column);
}

QString BgPlatform::detailData() const
{
    QString details;

    if (platformFlags == Smooth)
        details =  "Platform Movement: Smooth";
    else
        details =  "Platform Movement: Stepped";
    details += "\tVelocity: " + QString::number(vel);
    details += "\nClear Tile: " + clrTile;
    return details;
}

WorldItem::WorldItemType BgPlatform::type() const {
    return WorldItem::Platform;
}

bool BgPlatform::validChild(const WorldItem::WorldItemType&) const
{
    return false;
}

void BgPlatform::setData(const QVariant &data)
{
    platformData = data.toString();
}

qreal BgPlatform::offsetX() const
{
    WorldItem *p = this->parent();

    if (p)
        return p->offsetX() + p->relativeBoundingRect().left();
    else
        return 0.0;
}

BgPlatform::~BgPlatform()
{
     WorldItem::worldStats.platformCount--;
}
