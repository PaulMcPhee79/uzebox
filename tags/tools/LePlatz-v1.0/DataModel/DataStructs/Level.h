
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

#ifndef LEVEL_H
#define LEVEL_H

#include <QVariant>
#include <QString>
#include <PlatzGraphicsItem.h>
#include "WorldItem.h"

class Level : public WorldItem
{
public:
    Level();
    Level(const QList<QVariant> &data, WorldItem* parent = 0);
    ~Level();

    int mutableCount(int index = -1) const;
    WorldItem* createItem(const QList<QVariant> &data, WorldItem *parent = 0);
    QVariant data(int column) const;
    QVariant dataDecoration(int column) const;
    QVariant tooltipData(int column) const;
    void setData(const QVariant &data);
    bool validChild(const WorldItem::WorldItemType &type) const;
    WorldItem::WorldItemType type() const;
    qreal offsetX() const;

private:
    QString levelData;
};

#endif // LEVEL_H
