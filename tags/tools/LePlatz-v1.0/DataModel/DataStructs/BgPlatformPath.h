
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

#ifndef BGPLATFORMPATH_H
#define BGPLATFORMPATH_H

#include <QVariant>
#include <QString>
#include "WorldItem.h"
#include <PlatzGraphicsItem.h>

class BgPlatformPath : public WorldItem
{
public:
    enum PlatformAxis {
        AxisX = 0x01,
        AxisY = 0x02
    };

    static QString platformPathFlagsToString(const int &flags);

    BgPlatformPath();
    BgPlatformPath(const QList<QVariant> &data, WorldItem *parent = 0);
    ~BgPlatformPath();

    WorldItem* createItem(const QList<QVariant> &data, WorldItem *parent = 0);
    WorldItem* validateState();
    QVariant data(int column) const;
    QVariant dataDecoration(int column) const;
    QVariant tooltipData(int column) const;
    QString detailData() const;
    void setData(const QVariant &data);
    bool validChild(const WorldItem::WorldItemType &type) const;
    WorldItem::WorldItemType type() const;
    qreal offsetX() const;
    void setGraphicalRepresentation(PlatzGraphicsItem *item);
    PlatformAxis axis() { return platformAxis; }
    void setAxis(const PlatformAxis pa) { platformAxis = pa; }

private:
    QString platformPathData;
    PlatformAxis platformAxis;

};

#endif // BGPLATFORMPATH_H
