
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

#ifndef BGPLATFORM_H
#define BGPLATFORM_H

class BgPlatform : public WorldItem
{
public:

    enum PlatformStyle {    // clrTile should be in first 64 tiles so these don't get clobbered
        Smooth = 0x40,
        Stepped = 0x80
    };

    static QString platformFlagsToString(const int &flags);

    BgPlatform();
    BgPlatform(const QList<QVariant> &data, WorldItem *parent = 0);
    ~BgPlatform();

    WorldItem* createItem(const QList<QVariant> &data, WorldItem *parent = 0);
    QVariant data(int column) const;
    QVariant dataDecoration(int column) const;
    QVariant tooltipData(int column) const;
    QString detailData() const;
    void setData(const QVariant &data);
    bool validChild(const WorldItem::WorldItemType &type) const;
    WorldItem::WorldItemType type() const;
    qreal offsetX() const;
    WorldItem* validateState();

    QString clearTile() { return clrTile; }
    void setClearTile(const QString &tile) { clrTile = tile; }
    PlatformStyle flags() { return platformFlags; }
    void setFlags(PlatformStyle ps) { platformFlags = ps; }
    int velocity() { return vel; }
    void setVelocity(int v) { vel = v; }

private:
    QString platformData;
    QString clrTile;
    PlatformStyle platformFlags;
    int vel;
};

#endif // BGPLATFORM_H
