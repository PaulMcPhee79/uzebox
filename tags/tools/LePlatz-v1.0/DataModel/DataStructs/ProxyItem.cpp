
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
#include "ProxyItem.h"

ProxyItem::ProxyItem()
    : WorldItem(0)
{
    proxyData = "";
}

ProxyItem::ProxyItem(const QList<QVariant> &data, WorldItem::WorldItemType type, WorldItem* parent)
    : WorldItem(parent), proxyType(type)
{
    if (data.length() > 0)
        proxyData = data[0].toString();
    else
        proxyData = "";
    if (this->type() == WorldItem::OuterProxy)
        setMutableCount(mutableCount(row()));
}

WorldItem* ProxyItem::createItem(const QList<QVariant> &data, WorldItem *parent)
{
    ProxyItem *proxy = new ProxyItem(data, type(), (parent) ? parent : this->parent());

    if (proxy && graphicalRepresentation())
        proxy->setGraphicalRepresentation(new PlatzGraphicsItem(proxy, graphicalRepresentation()->mode()));
    return proxy;
}

QVariant ProxyItem::data(int column) const
{
    if (column == 0)
        return QVariant(proxyData);
    else
        return QVariant("");
}

QVariant ProxyItem::dataDecoration(int) const
{
    switch (proxyType) {
        case WorldItem::OuterProxy:
            return QVariant(WorldItem::worldItemIcon(WorldItem::OuterProxyIcon));
        case WorldItem::ObjectProxy:
            return QVariant(WorldItem::worldItemIcon(WorldItem::ObjectProxyIcon));
        case WorldItem::PlatformPathProxy:
            return QVariant(WorldItem::worldItemIcon(WorldItem::PlatformPathProxyIcon));
        default:
            return false;
    }
}

QVariant ProxyItem::tooltipData(int column) const
{
    return data(column);
}

WorldItem::WorldItemType ProxyItem::type() const {
    return proxyType;
}

bool ProxyItem::validChild(const WorldItem::WorldItemType &type) const
{
    switch (proxyType) {
        case WorldItem::OuterProxy:
            return type == WorldItem::Outer;
        case WorldItem::ObjectProxy:
            return type == WorldItem::Object;
        case WorldItem::PlatformPathProxy:
            return type == WorldItem::PlatformPath;
        default:
            return false;
    }
}

void ProxyItem::setData(const QVariant &data)
{
    proxyData = data.toString();
}

QRectF ProxyItem::boundingRect() const
{
    if (parent())
        return parent()->boundingRect();
    else
        return QRectF(0.0, 0.0, 0.0, 0.0);
}

QRectF ProxyItem::relativeBoundingRect() const
{
    if (parent())
        return parent()->relativeBoundingRect();
    else
        return QRectF(0.0, 0.0, 0.0, 0.0);
}

void ProxyItem::setBoundingRect(const QRectF &r)
{
    if (parent())
        parent()->setBoundingRect(r);
}

void ProxyItem::setRelativeBoundingRect(const QRectF &r)
{
    if (parent())
        parent()->setRelativeBoundingRect(r);
}

QRectF ProxyItem::limitRect() const
{
    if (parent())
        return parent()->boundingRect();
    else
        return QRectF(0.0, 0.0, 0.0, 0.0);
}

qreal ProxyItem::offsetX() const
{
    WorldItem *p = parent();

    if (p)
        return p->offsetX();
    else
        return 0.0;
}

ProxyItem::~ProxyItem()
{
}
