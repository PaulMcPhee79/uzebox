
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
#include "Level.h"

Level::Level()
    : WorldItem(0)
{
    levelData = "";
}

Level::Level(const QList<QVariant> &data, WorldItem* parent)
    : WorldItem(parent)
{
    if (data.length() > 0)
        levelData = data[0].toString();
    else
        levelData = "";
}

WorldItem* Level::createItem(const QList<QVariant> &data, WorldItem *parent)
{
    return new Level(data, (parent) ? parent : this->parent());
}

QVariant Level::data(int column) const
{
    if (column == 0)
        return QVariant(levelData);
    else
        return QVariant("");
}

QVariant Level::dataDecoration(int) const
{
    return QVariant(WorldItem::worldItemIcon(WorldItem::LevelIcon));
}

QVariant Level::tooltipData(int column) const
{
    int byteCount = WorldItem::sliceCount()*WorldItem::SLICE_BYTES +
                    WorldItem::outerCount()*WorldItem::OUTER_BYTES +
                    WorldItem::innerCount()*WorldItem::INNER_BYTES +
                    WorldItem::objectCount()*WorldItem::OBJECT_BYTES +
                    WorldItem::platformCount()*WorldItem::PLATFORM_BYTES;
    return QVariant(data(column).toString() +
                    "\nSlices: " + QString::number(WorldItem::sliceCount()) +
                    "\nBgOuters: " + QString::number(WorldItem::outerCount()) +
                    "\nBgInners: " + QString::number(WorldItem::innerCount()) +
                    "\nBgObjects: " + QString::number(WorldItem::objectCount()) +
                    "\nPlatformPaths: " + QString::number(WorldItem::platformPathCount()) +
                    "\nPlatforms: " + QString::number(WorldItem::platformCount()) +
                    "\nApprox Bytes: " + QString::number(byteCount));
}

WorldItem::WorldItemType Level::type() const {
    return WorldItem::Level;
}

int Level::mutableCount(int index) const
{
    if (index == -1)
        return WorldItem::mutableCount();    // Recursive calls end here

    int count = 0;
    index = qMin(index, childCount()-1);

    for (int i = index-1; i >= 0; i--)
        count += child(i)->mutableCount();
    return count;
}

bool Level::validChild(const WorldItem::WorldItemType &type) const
{
    return type == WorldItem::Slice;
}

void Level::setData(const QVariant &data)
{
    levelData = data.toString();
}

qreal Level::offsetX() const
{
    return 0.0;
}

Level::~Level()
{
}
