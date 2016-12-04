
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

#ifndef BGINNER_H
#define BGINNER_H

#include <QIcon>
#include <QPixmap>
#include <QVariant>
#include <QString>
#include "WorldItem.h"
#include <PlatzGraphicsItem.h>

class BgMutable;

class BgInner : public WorldItem
{
public:

    enum BgiFlags {
        BG0 = 0x00,
        BGP = 0x01,
        BGA = 0x02,
        BGM = 0x04  // Means next sibling is BGM info BgInner
    };

    static QString bgiFlagsToString(const int &flags);

    BgInner();
    BgInner(const QList<QVariant> &data, WorldItem *parent = 0);
    virtual ~BgInner();

    virtual WorldItem* validateState();
    virtual WorldItem* createItem(const QList<QVariant> &data, WorldItem *parent = 0);
    QVariant data(int column) const;
    QVariant dataDecoration(int column) const;
    QVariant tooltipData(int column) const;
    virtual QString detailData() const;
    void setData(const QVariant &data);
    bool validChild(const WorldItem::WorldItemType &type) const;
    WorldItem::WorldItemType type() const;
    virtual qreal offsetX() const;
    int tile() const { return bgTile; }
    virtual void setTile(int t);
    int flags() const { return bgFlags; }
    void setFlags(int f) { bgFlags = f; }
    BgInner* mutator() const { return bgMutator; }
    void setMutator(BgInner *mutator);    // Always set through this public interface (see ~BgMutable)

private:
    BgInner *bgMutator;
    QString innerData;
    int bgFlags;
    int bgTile; // Doubles as mutable id due to Platz level format
};

#endif // BGINNER_H
