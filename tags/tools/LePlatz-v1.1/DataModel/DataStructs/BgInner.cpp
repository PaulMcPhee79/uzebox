
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
#include "BgMutable.h"
#include "BgInner.h"

QString BgInner::bgiFlagsToString(const int &flags)
{
    QString retval;

    if (flags&BGP)
        retval += "BGP";
    if (flags&BGA) {
        if (retval.length())
            retval += "|";
        retval += "BGA";
    }
    if (flags&BGM) {
        if (retval.length())
            retval += "|";
        retval += "BGM";
    } else if (flags&BGMC) {
        if (retval.length())
            retval += "|";
        retval += "BGMC";
    }
    if (!retval.length())
        retval = "0";
    return retval;
}

BgInner::BgInner()
    : WorldItem(0), bgMutator(0), innerData(""), bgFlags(0), bgTile(-1), bgmc("")
{
    WorldItem::worldStats.innerCount++;
}

BgInner::BgInner(const QList<QVariant> &data, WorldItem* parent)
    : WorldItem(parent), bgMutator(0), innerData(""), bgFlags(0), bgTile(-1), bgmc("")
{
    if (data.length() > 0)
        innerData = data[0].toString();
    else
        innerData = "";
    WorldItem::worldStats.innerCount++;
}

WorldItem* BgInner::createItem(const QList<QVariant> &data, WorldItem *parent)
{
    BgInner *bgi = new BgInner(data, (parent) ? parent : this->parent());

    if (bgi) {
        bgi->setFlags(flags());
        bgi->setTile(tile());
        bgi->setBgmClass(bgmClass());
        bgi->setRelativeBoundingRect(relativeBoundingRect());

        if (graphicalRepresentation())
            bgi->setGraphicalRepresentation(new PlatzGraphicsItem(bgi, graphicalRepresentation()->mode()));
    }
    return bgi;
}

void BgInner::setFlags(int f)
{
    if ((f&BGMC) && (bgFlags&BGMC) == 0)
        incMutableCount();
    else if ((bgFlags&BGMC) && (f&BGMC) == 0)
        decMutableCount();
    bgFlags = f;
}

WorldItem* BgInner::validateState()
{
    if (!parent() || bgTile == -1 || ((bgFlags&BGMC) && bgmClass().isEmpty()))
        return this;
    if (bgFlags&BGM)    // BgInners with BGM set must be followed by and linked to a BgMutable
        if (row() == (parent()->childCount()-1) || !mutator() || mutator() != parent()->child(row()+1))
            return this;
    return 0;
}

QVariant BgInner::data(int column) const
{
    if (column == 0)
        return QVariant(innerData + " " + QString::number(row()));
    else
        return QVariant("");
}

QVariant BgInner::dataDecoration(int) const
{
    if (bgFlags&BGMC)
        return QVariant(WorldItem::worldItemIcon(WorldItem::MutableIcon));
    else if (bgFlags&BGA)
        return QVariant(WorldItem::worldItemIcon(WorldItem::AnimatedIcon));
    else if (bgFlags&BGP)
        return QVariant(WorldItem::worldItemIcon(WorldItem::PatternedIcon));
    else
        return QVariant(WorldItem::worldItemIcon(WorldItem::InnerIcon));
}

QVariant BgInner::tooltipData(int column) const
{
    if (bgFlags&BGMC)
        return QVariant(data(column).toString() + "\nBgm Class: " + bgmClass());
    else
        return QVariant(data(column).toString() + "\nTile Index: " + QString::number(bgTile));
}

QString BgInner::detailData() const
{
    QString details;

    details = "Bg Flags: " + bgiFlagsToString(bgFlags);

    if (bgFlags&BGMC) {
        details += "\tBgm Class: " + bgmClass();
        details += "\nMutable Index: " + QString::number(mutableCount());
    } else {
        if (bgFlags&BGP)
            details += "\nMap Index: ";
        else if (bgFlags&BGA)
            details += "\nAnim Index: ";
        else
            details += "\nTile Index: ";
        details += QString::number(bgTile);
    }
    return details;
}

WorldItem::WorldItemType BgInner::type() const
{
    return WorldItem::Inner;
}

bool BgInner::validChild(const WorldItem::WorldItemType&) const
{
    return false;
}

void BgInner::setData(const QVariant &data)
{
    innerData = data.toString();
}

void BgInner::setTile(int t)
{
    bgTile = t;
}

qreal BgInner::offsetX() const
{
    WorldItem *p = this->parent();

    if (p)
        return p->offsetX() + p->relativeBoundingRect().left();
    else
        return 0.0;
}

void BgInner::setMutator(BgInner *mutator)
{
    if (type() == WorldItem::Inner && bgMutator)
        bgMutator->setMutator(0);
    bgMutator = mutator;
}

int BgInner::mutableCount(int) const
{
    if (!parent())
        return ((bgFlags&BGMC)?1:0);
    return parent()->mutableCount(row());
}

BgInner::~BgInner()
{
    if (type() == WorldItem::Inner && bgMutator)
        bgMutator->setMutator(0);
    if (bgFlags&BGMC)
        decMutableCount();
    WorldItem::worldStats.innerCount--;
}
