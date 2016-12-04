
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

#ifndef WORLDCOMPILER_H
#define WORLDCOMPILER_H

#include <QIODevice>
#include <QTextStream>
#include <QRectF>
#include <QMap>
#include <QList>
#include <QString>
#include <WorldItem.h>
#include <PlatzDataModel.h>
#include <Slice.h>
#include <Level.h>
#include <BgOuter.h>
#include <ProxyItem.h>
#include <BgInner.h>
#include <BgMutable.h>
#include <BgObject.h>
#include <BgPlatformPath.h>
#include <BgPlatform.h>

class WorldCompiler
{
    struct BgDirectory {
        BgDirectory()
                : objOffset(0), objCount(0), bgoIndex(0), bgoCount(0), bgoBeginCount(0),
                bgoCommonCount(0), bgoEndIndex(0), bgiIndex(0), bgiCount(0) {}
        int objOffset;		// Index into pgmObjects flash array
        int objCount;		// The # of objects in the slice
        int bgoIndex;		// Index into pgmBgs flash array
        int bgoCount;		// The # of background elements in the slice
        int bgoBeginCount;	// The # of left-to-right seam collision bgs
        int bgoCommonCount;	// The # of right-to-left seam collision bgs common to begin/end
        int bgoEndIndex;	// Points to right-to-left seam collision bgs specific to end. Combined with bgoCommonIndex, prevents repeating bgs.
        int animCount;
        int animIndex;
        int platDirIndex;
        int bgiIndex;
        int bgiCount;
    };

    struct AnimMap {
        AnimMap(int outer, int inner)
                : outer(outer), inner(inner) {}
        int outer;
        int inner;
    };

    struct PlatformMap {
        PlatformMap(int index, int count)
                : index(index), count(count) {}
        int index;
        int count;
    };

public:
    WorldCompiler();
    WorldCompiler(WorldItem *root);
    ~WorldCompiler();
    bool compileWorld(QIODevice *device, const QString &worldName = "");
    void setRoot(WorldItem *rootItem);
    void setSliceSize(const QSize &size);
    void setSpriteSize(const QSize &size);
private:
    void clearLists();
    void compileBgObjects(int sliceIndex, ProxyItem *parent, QTextStream &ts);
    void compileAnimations(int sliceIndex, BgOuter *parent, QTextStream &ts);
    void compileBgInners(int sliceIndex, BgOuter *parent, QTextStream &ts);
    void compileBgOuters(int sliceIndex, ProxyItem *parent, QTextStream &ts);
    void compilePlatforms(int sliceIndex, ProxyItem *parent, QTextStream &ts);
    void compilePlatformDirectory(QTextStream &ts);
    void compileAnimations(QTextStream &ts);
    void compileBgDirectory(QTextStream &ts);
    QString rectFToString(const QRectF &rect, int shifted = 0);
    WorldItem *root;
    int platformIndex;
    QList<BgDirectory*> bgDir;
    QMap<int, QList<AnimMap*>*> animDir;
    QMap<int, PlatformMap*> platformDir;
    QSize sliceSize;
    QSize spriteSize;
};

#endif // WORLDCOMPILER_H
