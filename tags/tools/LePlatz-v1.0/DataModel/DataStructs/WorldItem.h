
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

#ifndef WORLDITEM_H
#define WORLDITEM_H

#include <QIcon>
#include <QRectF>
#include <QList>
#include <PlatzGraphicsItem.h>
#include <PlatzEnums.h>

class WorldItem
{
public:
    enum WorldItemType {
        Level = 0x01,
        Slice = 0x02,
        Outer = 0x04,
        OuterProxy = 0x08,
        Inner = 0x10,
        Mutable = 0x20,
        Object = 0x40,
        ObjectProxy = 0x80,
        PlatformPath = 0x100,
        PlatformPathProxy = 0x200,
        Platform = 0x400
    };

    static void loadWorldItemIcons();
    static void loadWorldDetailPixmaps();
    static const QPixmap& worldDetailPixmap(const WorldItemType &type);
    static QStringList mutableIds;
    static QStringList triggerIds;
    static QStringList platClrTileIds;

    static const int SLICE_BYTES = 12;
    static const int OUTER_BYTES = 8;
    static const int INNER_BYTES = 6;
    static const int OBJECT_BYTES = 3;
    static const int PLATFORM_BYTES = 9;

    static int sliceCount() { return WorldItem::worldStats.sliceCount; }
    static int outerCount() { return WorldItem::worldStats.outerCount; }
    static int innerCount() { return WorldItem::worldStats.innerCount; }
    static int objectCount() { return WorldItem::worldStats.objectCount; }
    static int platformPathCount() { return WorldItem::worldStats.platformPathCount; }
    static int platformCount() { return WorldItem::worldStats.platformCount; }

    static QString baseData(WorldItemType t) {
        switch (t) {
            case Level: return QString("Level");
            case Slice: return QString("Slice");
            case Outer: return QString("BgOuter");
            case Inner: return QString("BgInner");
            case Mutable: return QString("BgMutable");
            case Object: return QString("BgObject");
            case PlatformPath: return QString("PlatformPath");
            case Platform: return QString("Platform");
            case OuterProxy: return QString("Outer BGs");
            case ObjectProxy: return QString("BG Objects");
            case PlatformPathProxy: return QString("Moving Platforms");
            default: return QString("NULL");
        }
    }

    WorldItem();
    WorldItem(WorldItem *parent = 0);
    virtual ~WorldItem();

    void appendChild(WorldItem *child);
    void insertChild(int pos, WorldItem *child);
    WorldItem* child(int row) const;
    const QList<WorldItem*>* children(void);
    int childCount() const;
    virtual int columnCount() const;
    int row() const;
    WorldItem *parent() const;
    void setParent(WorldItem *parent);
    virtual WorldItem::WorldItemType type() const = 0;
    virtual int mutableCount(int index = -1) const;
    virtual void setMutableCount(int count) { mutableBgCount = count; }
    virtual void incMutableCount();
    virtual void decMutableCount();
    virtual WorldItem* validateState();

    // Textual View interface
    virtual WorldItem* createItem(const QList<QVariant> &data, WorldItem *parent = 0) = 0;
    virtual QVariant data(int column) const = 0;
    virtual QVariant dataDecoration(int column) const = 0;
    virtual QVariant tooltipData(int column) const = 0;
    virtual QString detailData() const;
    virtual void setData(const QVariant &data) = 0;
    //virtual void setColumnData(int column, int count, const QVariant &data) = 0;
    virtual void setChildRowData(int row, int count, const QVariant &data);
    virtual void setChildColumnData(int row, int column, int count, const QVariant &data);
    virtual bool removeChild(WorldItem *child);
    virtual bool removeChildRows(int row, int count);
    virtual bool validChild(const WorldItem::WorldItemType &type)const = 0;

    //Graphical View interface
    virtual qreal offsetX() const = 0;
    virtual QRectF boundingRect() const;
    virtual QRectF relativeBoundingRect() const;
    virtual void setBoundingRect(const QRectF &r);
    virtual void setRelativeBoundingRect(const QRectF &r);
    virtual QRectF limitRect() const;
    virtual void setDisplayMode(Platz::ItemDisplayMode mode);
    PlatzGraphicsItem* graphicalRepresentation();
    virtual void setGraphicalRepresentation(PlatzGraphicsItem *item);
protected:
    static struct WorldStats {
        int sliceCount;
        int outerCount;
        int innerCount;
        int objectCount;
        int platformPathCount;
        int platformCount;
    } worldStats;

    enum WorldItemIconIndex {
        LevelIcon,
        SliceIcon,
        OuterProxyIcon,
        OuterIcon,
        TriggerIcon,
        MutableIcon,
        InnerIcon,
        AnimatedIcon,
        PatternedIcon,
        ObjectProxyIcon,
        ObjectIcon,
        PlatformPathProxyIcon,
        PlatformPathIcon,
        PlatformIcon,
        QueryIcon
    };

    static const QIcon& worldItemIcon(const WorldItemIconIndex &index);
private:
    static QList<QIcon> WorldItemIcons;
    static QList<QPixmap> WorldDetailPixmaps;

    QList<WorldItem*> childItems;
    PlatzGraphicsItem *graphicsItem;
    WorldItem *parentItem;
    QRectF bounds;
    int mutableBgCount;
};

#endif // WORLDITEM_H
