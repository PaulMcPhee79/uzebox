
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

#ifndef BGOUTER_H
#define BGOUTER_H

#include <QVariant>
#include <QString>
#include <PlatzGraphicsItem.h>

class BgOuter : public WorldItem
{
public:

    enum BgoFlags {
        BGC = 0x01,
        BGT = 0x02,
        BGI = 0x04,
        BGM = 0x08,
        BGQ = 0x10,
        BGPRJ = 0x20
    };

    static QString bgoFlagsToString(const int &flags);

    BgOuter();
    BgOuter(const QList<QVariant> &data, WorldItem* parent = 0);
    ~BgOuter();

    int mutableCount(int index = -1) const;

    WorldItem* createItem(const QList<QVariant> &data, WorldItem *parent = 0);
    QVariant data(int column) const;
    QVariant dataDecoration(int column) const;
    QVariant tooltipData(int column) const;
    QString detailData() const;
    void setData(const QVariant &data);
    bool validChild(const WorldItem::WorldItemType &type) const;
    WorldItem::WorldItemType type() const;
    qreal offsetX() const;
    int flags() { return bgFlags; }
    void setFlags(int f) { bgFlags = f; }
    int trigger() { return trigIndex; }
    QString triggerStr() const;
    void setTrigger(int t) { trigIndex = t; }
    int triggerOrientation() { return trigOrientation; }
    QString triggerOrientationStr() const;
    void setTriggerOrientation(int to) { trigOrientation = to; }
    void setGraphicalRepresentation(PlatzGraphicsItem *item);
private:
    QString bgOuterData;
    int bgFlags;
    int trigIndex;
    int trigOrientation;
};

#endif // BGOUTER_H
