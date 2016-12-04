
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

#ifndef BGMUTABLE_H
#define BGMUTABLE_H

#include <PlatzEnums.h>
#include "BgInner.h"

class BgMutable : public BgInner
{
public:
    BgMutable();
    BgMutable(const QList<QVariant> &data, BgInner *mutableLink = 0, WorldItem *parent = 0);
    ~BgMutable();

    virtual WorldItem* validateState();
    WorldItem* createItem(const QList<QVariant> &data, WorldItem *parent = 0);
    WorldItem::WorldItemType type() const;
    QVariant dataDecoration(int column) const;
    QString detailData() const;
    QRectF limitRect() const;
    qreal offsetX() const;
    QString mutableString() const;
    bool isCustomPayload() { return customPayload; }
    const Platz::MutablePayload& payload() const { return mutablePayload; }
    void setPayload(const QRect &rect) { mutablePayload.fromRect(rect); customPayload = false; }
    void setCustomPayload(const Platz::MutablePayload& payload) { mutablePayload = payload; customPayload = true; }
    int mutableCount(int index = -1) const;
private:
    Platz::MutablePayload mutablePayload;
    bool customPayload;
};

#endif // BGMUTABLE_H
