
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

#include <QString>
#include <QRectF>
#include "WorldItem.h"
#include "BgOuter.h"
#include "BgInner.h"

QString BgOuter::bgoFlagsToString(const int &flags)
{
    QString retval;

    if (flags&BGC)
        retval += "BGC";
    if (flags&BGQ) {
        if (retval.length())
            retval += "|";
        retval += "BGQ";
    }
    if (flags&BGI) {
        if (retval.length())
            retval += "|";
        retval += "BGI";
    }
    if (flags&BGM) {
        if (retval.length())
            retval += "|";
        retval += "BGM";
    }
    if (flags&BGT) {
        if (retval.length())
            retval += "|";
        retval += "BGT";
    }
    if (flags&BGPRJ) {
        if (retval.length())
            retval += "|";
        retval += "BGPRJ";
    }
    if (!retval.length())
        retval = "0";
    return retval;
}

BgOuter::BgOuter()
    : WorldItem(0), bgFlags(0)
{
    bgOuterData = "";
    WorldItem::worldStats.outerCount++;
    setMutableCount(mutableCount(row()));
}

BgOuter::BgOuter(const QList<QVariant> &data, WorldItem* parent)
    : WorldItem(parent),  bgOuterData(""), bgFlags(0), mTrigger(""), mTriggerOrientation("")
{
    if (data.length() > 0)
        bgOuterData = data[0].toString();

    WorldItem::worldStats.outerCount++;
    setMutableCount(mutableCount(row()));
}

WorldItem* BgOuter::createItem(const QList<QVariant> &data, WorldItem *parent)
{
    BgOuter *bgo = new BgOuter(data, (parent) ? parent : this->parent());

    if (bgo) {
        bgo->setFlags(flags());
        bgo->setTrigger(trigger());
        bgo->setTriggerOrientation(triggerOrientation());
        bgo->setRelativeBoundingRect(relativeBoundingRect());

        if (graphicalRepresentation())
            bgo->setGraphicalRepresentation(new PlatzGraphicsItem(bgo, graphicalRepresentation()->mode()));
    }
    return bgo;
}

QVariant BgOuter::data(int column) const
{
    if (column == 0)
        return QVariant(bgOuterData + " " + QString::number(row()));
    else
        return QVariant("");
}

QVariant BgOuter::dataDecoration(int) const
{
    if (bgFlags&BGT)
        return QVariant(WorldItem::worldItemIcon(WorldItem::TriggerIcon));
    else if (bgFlags&BGQ)
        return QVariant(WorldItem::worldItemIcon(WorldItem::QueryIcon));
    else
        return QVariant(WorldItem::worldItemIcon(WorldItem::OuterIcon));
}

QVariant BgOuter::tooltipData(int column) const
{
    QRectF r = relativeBoundingRect();
    QString s = QString("%1 : {%2,%3,%4,%5}").arg(data(column).toString()).arg(r.left()).arg(r.right()).arg(r.top()).arg(r.bottom());
    return QVariant(s);
}

QString BgOuter::detailData() const
{
    QString details;

    details = "Bg Flags: " + bgoFlagsToString(bgFlags);

    if (bgFlags&BGT) {
        details += "\nTrigger Index: " + trigger();
        details +=  + "\tTrigger Orientation: " + triggerOrientation();
    }
    return details;
}

WorldItem::WorldItemType BgOuter::type() const {
    return WorldItem::Outer;
}

void BgOuter::setData(const QVariant &data)
{
    bgOuterData = data.toString();
}

bool BgOuter::validChild(const WorldItem::WorldItemType&) const
{
    return false;
}

qreal BgOuter::offsetX() const
{
    WorldItem *p = this->parent();

    if (p)
        return p->offsetX();
    else
        return 0.0;
}

void BgOuter::setGraphicalRepresentation(PlatzGraphicsItem *item)
{
    WorldItem::setGraphicalRepresentation(item);

    if (item)
        item->setZValue(1.0);
}

int BgOuter::mutableCount(int index) const
{
    if (index == -1)
        return WorldItem::mutableCount();    // Recursive calls end here

    int count = 0;
    index = qMin(index, childCount()-1);

    for (int i = index-1; i >= 0; i--) {
        if (child(i)->type() == WorldItem::Mutable) {
            ++count;
        } else if (child(i)->type() == WorldItem::Inner) {
            BgInner *bgi = static_cast<BgInner*>(child(i));

            if (bgi->flags()&BgInner::BGMC)
                ++count;
        }
    }
    if (parent())
        count += parent()->mutableCount(row());
    return count;
}

WorldItem* BgOuter::validateState()
{
    if ((flags()&BGT) && (trigger().isEmpty() || triggerOrientation().isEmpty()))
        return this;
    return WorldItem::validateState();
}

BgOuter::~BgOuter()
{
     WorldItem::worldStats.outerCount--;
}
