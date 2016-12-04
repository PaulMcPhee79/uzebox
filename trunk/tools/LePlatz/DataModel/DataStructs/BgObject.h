
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

#ifndef BGOBJECT_H
#define BGOBJECT_H

#include <QVariant>
#include <QString>
#include "WorldItem.h"
#include <PlatzGraphicsItem.h>

class BgObject : public WorldItem
{
public:
    BgObject();
    BgObject(const QList<QVariant> &data, WorldItem *parent = 0);
    ~BgObject();

    WorldItem* createItem(const QList<QVariant> &data, WorldItem *parent = 0);
    QVariant data(int column) const;
    QVariant dataDecoration(int column) const;
    QVariant tooltipData(int column) const;
    QString detailData() const;
    void setData(const QVariant &data);
    bool validChild(const WorldItem::WorldItemType &type) const;
    WorldItem::WorldItemType type() const;
    qreal offsetX() const;
    int map() const { return bgMap; }
    void setMap(int m) { bgMap = m; }

private:
    QString objectData;
    int bgMap;    // Index into maps table
};

#endif // BGOBJECT_H
