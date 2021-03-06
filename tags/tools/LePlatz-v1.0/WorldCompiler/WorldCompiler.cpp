
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

#include <QDateTime>
#include "WorldCompiler.h"

 WorldCompiler::WorldCompiler()
    : root(0), platformIndex(0)
 {

 }

WorldCompiler::WorldCompiler(WorldItem *root)
    : root(root), platformIndex(0), sliceSize(QSize(0, 0)), spriteSize(QSize(0, 0))
{
}

void WorldCompiler::setRoot(WorldItem *rootItem)
{
    root = rootItem;
}

void WorldCompiler::setSliceSize(const QSize &size)
{
    if (size.isValid())
        sliceSize = size;
}

void WorldCompiler::setSpriteSize(const QSize &size)
{
    if (size.isValid())
        spriteSize = size;
}

bool WorldCompiler::compileWorld(QIODevice *device, const QString &worldName)
{
    if (!root || !device || !device->isOpen() || !device->isWritable())
        return false;
    bool postfix = false;
    QTextStream ts(device);
    Slice *slice = 0;
    clearLists();
    platformIndex = 0;

    ts << "/********************************************************************************\n"
        << "** Platz world data file generated by LePlatz " << Platz::VERSION_STR
        << "\n**\n"
        << "** Created: " << QDateTime::currentDateTime().toString(Qt::TextDate)
        << "\n**\n"
        << "** WARNING! All changes made in this file will be lost when recompiling " << worldName << ".xml\n"
        << "********************************************************************************/\n\n";

    ts << "const object pgmObjects[] PROGMEM = {\n";    // BgObjects

    for (int i = 0; i < root->childCount(); i++) {
        bgDir.append(new BgDirectory());
        bgDir[i]->animCount = 0;
        bgDir[i]->animIndex = 0;
        bgDir[i]->platDirIndex = -1;    // PF_ZERO

        if (i)
            bgDir[i]->objOffset = bgDir[i-1]->objOffset + bgDir[i-1]->objCount;
        else
            bgDir[i]->objOffset = 0;
        slice = static_cast<Slice*>(root->child(i));

        if (!slice || slice->replicaOf())
            continue;
        ProxyItem *proxy = slice->objectProxy();

        if (proxy->childCount()) {
            if (postfix && root->child(i)->row())          // Postfix previous item if not first item
                ts << ",\n";
            compileBgObjects(i, proxy, ts);
            postfix = true;
        }
    }
    ts << "\n};\n\n";
    ts << "const bgInner pgmBgsInner[] PROGMEM = {\n";                          // BgInners
    postfix = false;

    for (int i = 0; i < root->childCount(); i++) {
        if (i)
            bgDir[i]->animIndex = bgDir[i-1]->animIndex + bgDir[i-1]->animCount;
        else
            bgDir[i]->animIndex = 0;
        slice = static_cast<Slice*>(root->child(i));

        if (!slice || slice->replicaOf())
            continue;
        ProxyItem *proxy = slice->outerProxy();

        foreach (WorldItem *proxyChild, *proxy->children()) {
            BgOuter *bgo = static_cast<BgOuter*>(proxyChild);

            if (bgo->childCount()) {
                if (postfix && (root->child(i)->row() || bgo->row()))
                    ts << ",\n";
                compileBgInners(i, bgo, ts);
                postfix = true;
            }
        }
    }
    ts << "\n};\n\n";
    ts << "const bgOuter pgmBgsOuter[] PROGMEM = {\n";                          // BgOuters
    postfix = false;

    for (int i = 0; i < root->childCount(); i++) {
        if (i) {
            bgDir[i]->bgoIndex = bgDir[i-1]->bgoIndex + bgDir[i-1]->bgoCount;
            bgDir[i]->bgiIndex = bgDir[i-1]->bgiIndex + bgDir[i-1]->bgiCount;
        } else {
            bgDir[i]->bgoIndex = 0;
            bgDir[i]->bgiIndex = 0;
        }
        slice = static_cast<Slice*>(root->child(i));

        if (!slice || slice->replicaOf())
            continue;
        ProxyItem *proxy = slice->outerProxy();
        bgDir[i]->bgoCount = proxy->childCount();

        if (proxy->childCount()) {
            if (postfix && root->child(i)->row())
                ts << ",\n";
            compileBgOuters(i, proxy, ts);
            postfix = true;
        }
    }
    ts << "\n};\n\n";
    ts << "const platform pgmPlatforms[] PROGMEM = {\n";                        // Platforms
    postfix = false;

    for (int i = 0, j = 0; i < root->childCount(); i++) {
        slice = static_cast<Slice*>(root->child(i));

        if (!slice || slice->replicaOf())
            continue;
        ProxyItem *proxy = slice->platformProxy();

        if (proxy->childCount()) {
            if (postfix && root->child(i)->row())
                ts << ",\n";
            compilePlatforms(i, proxy, ts);
            postfix = true;
        }
        bgDir[i]->platDirIndex = (platformDir.contains(i)?j++:-1);
    }
    ts << "\n};\n\n";
    compilePlatformDirectory(ts);
    compileAnimations(ts);
    compileBgDirectory(ts);
    ts.flush();
    return true;
}

void WorldCompiler::compileBgDirectory(QTextStream &ts)
{
    int sliceIndex = 0;
    BgDirectory *dir;
    Slice *slice;

    ts << "const bgDirectory pgmBgDir[] PROGMEM = {\n";

    for (int i = 0; i < bgDir.count(); i++) {
        slice = static_cast<Slice*>(root->child(i));

        if (!slice)
            continue;
        if (slice->replicaOf())
            dir = bgDir[slice->replicaOf()->row()];
        else
            dir = bgDir[i];
        ts << "\t{" << dir->objOffset << ","
                << dir->objCount << ","
                << dir->bgoIndex << ","
                << dir->bgoCount << ","
                << dir->bgoBeginCount << ","
                << dir->bgoCommonCount << ","
                << dir->bgoEndIndex << ","
                << dir->animCount << ","
                << dir->animIndex << ",";
        if (dir->platDirIndex != -1)
            ts << dir->platDirIndex << "}";
        else
            ts << "PF_ZERO}";
        if (++sliceIndex != bgDir.count())
            ts << ",\n";
    }
    ts << "\n};\n";

/*
    int sliceIndex = 0, animCount = 0, animIndex = 0, bgDirPlatIndex = 0;

    ts << "const bgDirectory pgmBgDir[] PROGMEM = {\n";

    foreach (BgDirectory *dir, bgDir) {
        animCount = (animDir.contains(sliceIndex)?animDir[sliceIndex]->count():0);

        ts << "\t{" << dir->objOffset << ","
                << dir->objCount << ","
                << dir->bgoIndex << ","
                << dir->bgoCount << ","
                << dir->bgoBeginCount << ","
                << dir->bgoCommonCount << ","
                << dir->bgoEndIndex << ","
                << animCount << ","
                << animIndex << ",";
        if (platformDir.contains(sliceIndex))
            ts << bgDirPlatIndex++ << "}";
        else
            ts << "PF_ZERO" << "}";
        if (++sliceIndex != bgDir.count())
            ts << ",\n";
        animIndex += animCount;
    }
    ts << "\n};\n";
*/
}

void WorldCompiler::compileAnimations(QTextStream &ts)
{
    int i = 0;

    ts << "const bgAnimIndex pgmAnimDir[] PROGMEM = {\n";

    foreach (QList<AnimMap*> *mapList, animDir) {
        if (mapList->count())
            ts << "\t";
        foreach (AnimMap *am, *mapList) {
            ts << "{" << am->outer << "," << am->inner << "}";
            if (mapList->indexOf(am) != (mapList->count()-1))
                ts << ",";
        }
        if (++i != animDir.count())
            ts << ",\n";
    }
    ts << "\n};\n\n";
}

void WorldCompiler::compilePlatformDirectory(QTextStream &ts)
{
    int i = 0;
    ts << "const platformDirectory pgmPlatformDir[] PROGMEM = {\n";

    foreach (PlatformMap *pm, platformDir) {
        ts << "\t{" << pm->index << "," << pm->count << "}";

        if (i++ != (platformDir.count()-1))
            ts << ",\n";
    }
    ts << "\n};\n\n";
}

void WorldCompiler::compilePlatforms(int sliceIndex, ProxyItem *parent, QTextStream &ts)
{
    int platCount = 0;
    QRectF pathRect;

    foreach (WorldItem *child, *parent->children()) {
        BgPlatformPath *platPath = static_cast<BgPlatformPath*>(child);

        pathRect = platPath->relativeBoundingRect();

        foreach (WorldItem *platChild, *platPath->children()) {
            BgPlatform *plat = static_cast<BgPlatform*>(platChild);
            ts << "\t{" << plat->clearTile() << "|"
                    << BgPlatform::platformFlagsToString(plat->flags()) << ","
                    << ((platPath->axis() == BgPlatformPath::AxisX)?(int)platPath->relativeBoundingRect().left()
                        :(int)platPath->relativeBoundingRect().top()) << ","
                    << ((platPath->axis() == BgPlatformPath::AxisX)?(int)platPath->relativeBoundingRect().right()-plat->relativeBoundingRect().width()
                        :(int)platPath->relativeBoundingRect().bottom()-plat->relativeBoundingRect().height()) << ","
                    << BgPlatformPath::platformPathFlagsToString(platPath->axis()) << ","
                    << plat->velocity() << ","
                    << rectFToString(plat->relativeBoundingRect().adjusted(pathRect.left(), 0, pathRect.left(), 0)) << "}";
            if (plat->row() != (platPath->childCount()-1))
                ts << ",\n";
            ++platCount;
        }
        if (platPath->row() != (parent->childCount()-1))
            ts << ",\n";
    }
    if (platCount) {
        if (!platformDir.contains(sliceIndex))
            platformDir.insert(sliceIndex, new PlatformMap(platformIndex, platCount));
        platformIndex += platCount;
    }
}

void WorldCompiler::compileBgOuters(int sliceIndex, ProxyItem *parent, QTextStream &ts)
{
    int index = 0;

    foreach (WorldItem *child, *parent->children()) {
        BgOuter *bgo = static_cast<BgOuter*>(child);
        ts << "\t{" + BgOuter::bgoFlagsToString(bgo->flags()) << ",";

        if (bgo->flags()&BgOuter::BGT)
            ts << bgo->triggerOrientationStr() << "," << bgo->triggerStr();
        else
            ts << bgo->childCount() << "," << bgDir[sliceIndex]->bgiIndex+bgDir[sliceIndex]->bgiCount;
        ts << "," << rectFToString(bgo->relativeBoundingRect(), 3) << "}";

        if (bgo->row() != (parent->childCount()-1))
            ts << ",\n";
        bgDir[sliceIndex]->bgiCount += bgo->childCount();

        // Uncommenting the outer conditional would optimize collision detection very slightly, but it
        // also makes building levels more cumbersome for various reasons.
        //if (bgo->flags()&(BgOuter::BGC|BgOuter::BGM|BgOuter::BGQ|BgOuter::BGT)) {
            if (bgo->relativeBoundingRect().left() <= spriteSize.width()) {
                bgDir[sliceIndex]->bgoBeginCount++;

                if ((sliceSize.width() - bgo->relativeBoundingRect().right()) <= spriteSize.width())
                    bgDir[sliceIndex]->bgoCommonCount++;
            } else if ((bgDir[sliceIndex]->bgoEndIndex == 0) && (sliceSize.width() - bgo->relativeBoundingRect().right()) <= spriteSize.width()) {
                bgDir[sliceIndex]->bgoEndIndex = index;
            }
        //}
        ++index;
    }
}

void WorldCompiler::compileBgInners(int sliceIndex, BgOuter *parent, QTextStream &ts)
{
    QRectF bgoRect;

    foreach (WorldItem *child, *parent->children()) {
        BgInner *bgi = static_cast<BgInner*>(child);

        if (child->type() == WorldItem::Inner) {
            bgoRect = parent->relativeBoundingRect();
            ts << "\t{" << BgInner::bgiFlagsToString(bgi->flags()) << "," << bgi->tile() << "," <<
                    rectFToString(bgi->relativeBoundingRect().adjusted(bgoRect.left(), 0, bgoRect.left(), 0), 3) << "}";
        } else {    // type() == Mutable
            BgMutable *bgm = static_cast<BgMutable*>(child);

            ts << "\t{" << bgm->mutableCount() << "," << bgm->mutableString() << ",";

            if (bgm->isCustomPayload())
                ts << bgm->payload().toString() << "}";
            else
                ts << bgm->payload().toString(3) << "}";
        }
        if (bgi->flags()&BgInner::BGA) {    // Update animations map (we'll print them later)
            if (!animDir.contains(sliceIndex))
                animDir.insert(sliceIndex, new QList<AnimMap*>);
            animDir[sliceIndex]->append(new AnimMap(parent->row(), bgi->row()));
        }
        if (bgi->row() != (parent->childCount()-1))
            ts << ",\n";
    }
    bgDir[sliceIndex]->animCount = (animDir.contains(sliceIndex)?animDir[sliceIndex]->count():0);
}

void WorldCompiler::compileBgObjects(int sliceIndex, ProxyItem *parent, QTextStream &ts)
{
    foreach (WorldItem *child, *parent->children()) {
        BgObject *obj = static_cast<BgObject*>(child);
        ts << "\t{{" << ((int)obj->relativeBoundingRect().left()>>3)
                << "," << ((int)obj->relativeBoundingRect().top()>>3)
                << "}," << obj->map() << "}";
        if (obj->row() != (parent->childCount()-1))
            ts << ",\n";
        ++bgDir[sliceIndex]->objCount;
    }
}

QString WorldCompiler::rectFToString(const QRectF &rect, int shifted)
{
    return "{" + QString::number((int)rect.left()>>shifted) + "," + QString::number((int)rect.right()>>shifted) + "," +
            QString::number((int)rect.top()>>shifted) + "," + QString::number((int)rect.bottom()>>shifted) + "}";
}

void WorldCompiler::clearLists()
{
    while (bgDir.count())
        delete bgDir.takeAt(0);
    while (animDir.count()) {
        QList<int> keys = animDir.keys();

        foreach (int k, keys) {
            QList<AnimMap*> *maps = animDir.take(k);

            while (maps->count())
                delete maps->takeAt(0);
        }
    }
    QList<int> keys = platformDir.keys();

    foreach (int k, keys) {
        delete platformDir.take(k);
    }
}

WorldCompiler::~WorldCompiler()
{
    clearLists();
}
