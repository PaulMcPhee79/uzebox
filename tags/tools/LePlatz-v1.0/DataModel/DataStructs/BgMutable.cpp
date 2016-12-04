
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

#include "BgMutable.h"

BgMutable::BgMutable()
{
}

BgMutable::BgMutable(const QList<QVariant> &data, BgInner *mutableLink, WorldItem *parent)
    : BgInner(data, parent), customPayload(false)
{
    setMutator(mutableLink);
    incMutableCount();
}

WorldItem* BgMutable::createItem(const QList<QVariant> &data, WorldItem *parent)
{
    BgMutable *bgm = new BgMutable(data, mutator(), (parent) ? parent : this->parent());

    if (bgm) {
        bgm->setFlags(flags());
        bgm->setTile(tile());
        bgm->setRelativeBoundingRect(relativeBoundingRect());

        if (customPayload)
            bgm->setCustomPayload(mutablePayload);
        else
            bgm->setPayload(mutablePayload.toRect());

        if (graphicalRepresentation())
            bgm->setGraphicalRepresentation(new PlatzGraphicsItem(bgm, graphicalRepresentation()->mode()));
    }
    return bgm;
}

WorldItem* BgMutable::validateState()
{
    // BgMutables must follow a BgInner and be linked to it
    if (!parent() || !row() || !mutator() || mutator() != parent()->child(row()-1))
        return this;
    return 0;
}

WorldItem::WorldItemType BgMutable::type() const
{
    return WorldItem::Mutable;
}

QVariant BgMutable::dataDecoration(int) const
{
    return QVariant(WorldItem::worldItemIcon(WorldItem::MutableIcon));
}

QString BgMutable::detailData() const
{
    return "Mutable Bg Id: " + mutableString() + "\tMutable Index: " + QString::number(mutableCount()) +
            "\nPayload: " + QString::number(mutablePayload.left) + "," +
            QString::number(mutablePayload.right) + "," +
            QString::number(mutablePayload.top) + "," +
            QString::number(mutablePayload.btm);
}

QString BgMutable::mutableString() const
{
    if (tile() >= 0 && WorldItem::mutableIds.count() > tile())
        return WorldItem::mutableIds.at(tile());
    else
        return Platz::UNDEFINED;
}

QRectF BgMutable::limitRect() const
{
    if (parent() && parent()->parent())
        return parent()->parent()->boundingRect();
    else
        return QRectF(0.0, 0.0, 0.0, 0.0);
}

qreal BgMutable::offsetX() const
{
    if (mutator() && mutator()->parent())
        return mutator()->parent()->offsetX();
    else
        return 0.0;
}

int BgMutable::mutableCount(int) const
{
    if (!parent())
        return 1;
    return parent()->mutableCount(row());
}

BgMutable::~BgMutable()
{
    decMutableCount();

    if (mutator())
        mutator()->setMutator(0);
}
