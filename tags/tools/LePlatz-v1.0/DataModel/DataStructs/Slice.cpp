
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

#include <QList>
#include "WorldItem.h"
#include "Slice.h"

Slice::Slice()
    : WorldItem(0), replica(0), locked(false)
{
    sliceData = "";
    WorldItem::worldStats.sliceCount++;
    setMutableCount(mutableCount(row()));
}

Slice::Slice(const QList<QVariant> &data, WorldItem* parent)
    : WorldItem(parent), replica(0), locked(false)
{
    if (data.length() > 0)
        sliceData = data[0].toString();
    else
        sliceData = "";
    WorldItem::worldStats.sliceCount++;
    setMutableCount(mutableCount(row()));
}

WorldItem* Slice::createItem(const QList<QVariant> &data, WorldItem *parent)
{
    Slice *slice = new Slice(data, (parent) ? parent : this->parent());

    if (slice) {
        slice->setReplica(replica);
        slice->setLockedOrdering(locked);

        if (graphicalRepresentation())
            slice->setGraphicalRepresentation(new PlatzGraphicsItem(slice, graphicalRepresentation()->mode()));
    }
    return slice;
}

QVariant Slice::data(int column) const
{
    if (column == 0)
        return QVariant(sliceData + " " + QString::number(row()));
    else
        return QVariant("");
}

QVariant Slice::dataDecoration(int) const
{
    return QVariant(WorldItem::worldItemIcon(WorldItem::SliceIcon));
}

QVariant Slice::tooltipData(int) const
{
    return QVariant(data(0).toString() +
            "\nLocked: " + ((locked)?"Yes":"No") +
            "\nBgOuters: " + QString::number(outerProxy()->childCount()) +
            "\nBgObjects: " + QString::number(objectProxy()->childCount()) +
            "\nBgPlatformPaths: " + QString::number(platformProxy()->childCount()));
}

QString Slice::detailData() const
{
    QString details((locked)?"Locked: Yes":"Locked: No");
    details += "\nReplica Of: " + ((replica) ? replica->data(0).toString() : "None");
    return details;
}

WorldItem::WorldItemType Slice::type() const {
    return WorldItem::Slice;
}

bool Slice::validChild(const WorldItem::WorldItemType&) const
{
    return false;
}

void Slice::setData(const QVariant &data)
{
    sliceData = data.toString();
}

QRectF Slice::limitRect() const
{
    return boundingRect();
}

qreal Slice::offsetX() const
{
    WorldItem *p = parent();

    if (p)
        return p->offsetX() + row()*256.0;
    else
        return 0.0;
}

ProxyItem* Slice::outerProxy() const
{
    if (childCount())
        return reinterpret_cast<ProxyItem*>(child(0));
    else
        return 0;
}

ProxyItem* Slice::objectProxy() const
{
    if (childCount() > 0)
        return reinterpret_cast<ProxyItem*>(child(1));
    else
        return 0;
}

ProxyItem* Slice::platformProxy() const
{
    if (childCount() > 1)
        return reinterpret_cast<ProxyItem*>(child(2));
    else
        return 0;
}

Slice::~Slice()
{
    // Resolve any potentially stray pointers
    if (parent()) {
        Slice *it;

        foreach (WorldItem *child, *parent()->children()) {
            it = static_cast<Slice*>(child);

            if (it->replicaOf() == this)
                it->setReplica(0);
        }
    }

    WorldItem::worldStats.sliceCount--;

    // Not managed by the scene. Also shared with proxy types.
    delete graphicalRepresentation();
    setGraphicalRepresentation(0);
}


