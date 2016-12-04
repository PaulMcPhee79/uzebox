
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

#ifndef SLICE_H
#define SLICE_H

#include <QVariant>
#include <QString>
#include <PlatzGraphicsItem.h>
#include <ProxyItem.h>

class Slice : public WorldItem
{
public:
    Slice();
    Slice(const QList<QVariant> &data, WorldItem* parent = 0);
    ~Slice();

    WorldItem* createItem(const QList<QVariant> &data, WorldItem *parent = 0);
    QVariant data(int column) const;
    QVariant dataDecoration(int column) const;
    QVariant tooltipData(int column) const;
    QString detailData() const;
    void setData(const QVariant &data);
    bool validChild(const WorldItem::WorldItemType &type) const;
    WorldItem::WorldItemType type() const;
    QRectF limitRect() const;
    qreal offsetX() const;
    ProxyItem* outerProxy() const;
    ProxyItem* objectProxy() const;
    ProxyItem* platformProxy() const;
    const Slice* replicaOf() { return replica; }
    void setReplica(const Slice *slice) { replica = slice; }
    bool lockedOrdering() const { return locked; }
    void setLockedOrdering(bool lockState) { locked = lockState; }
    int bgoOrder() { return mBgoOrder; }
    //void toggleBgoOrder();
    //void setBgoOrder(int order){  mBgoOrder = order; }  // Don't like how model sorts afterwards
    int validateBgoOrdering();
    //void setColumnData(int column, int count, const QVariant &data);

private:
    const Slice *replica;
    QString sliceData;
    bool locked;
    int mBgoOrder;   // 1 = left-to-right; -1 = right-to-left
};

#endif // SLICE_H
