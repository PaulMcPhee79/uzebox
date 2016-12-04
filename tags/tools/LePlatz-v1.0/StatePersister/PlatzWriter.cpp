
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

#include <QFile>
#include <WorldItem.h>
#include <Slice.h>
#include <BgOuter.h>
#include <BgInner.h>
#include <BgMutable.h>
#include <BgObject.h>
#include <BgPlatformPath.h>
#include <BgPlatform.h>
#include "PlatzWriter.h"


PlatzWriter::PlatzWriter(Settings *settings, WorldItem *root)
    : settings(settings), root(root)
{
    setAutoFormatting(true);
}

bool PlatzWriter::saveWorld(const QString &path)
{
    if (!settings || !root)
        return false;
    QFile file(path);

    if (!file.open(QFile::WriteOnly | QFile::Text))
        return false;
    setDevice(&file);
    writeStartDocument();
    writeDTD("<!DOCTYPE LePlatz>");
    writeStartElement("LePlatz");
    writeAttribute("version", "1.0");
    writeSettings();
    writeStartElement("World");

    foreach(WorldItem *child, *root->children())
        writeItem(child);
    writeEndDocument();
    return true;
}

bool PlatzWriter::saveLePlatzSettings(const QString &path, const QByteArray &winGeometry, const QByteArray &winLayout)
{
    if (!settings)
        return false;
    QFile file(path);

    if (!file.open(QFile::WriteOnly | QFile::Text))
        return false;
    setDevice(&file);
    writeStartDocument();
    writeDTD("<!DOCTYPE LePlatzSettings>");
    writeStartElement("LePlatz");
    writeAttribute("version", "1.0");
    writeStartElement("LePlatzSettings");
    writeTextElement("ScreenLayout", QString(winGeometry.toHex()));
    writeTextElement("WinLayout", QString(winLayout.toHex()));
    writeTextElement("MakeExePath", settings->makeExePath());
    writeTextElement("EmuExePath", settings->emuExePath());

    if (settings->recentProjects().count()) {
        writeStartElement("RecentProjects");

        // Store in reverse so that we load through add interface
        for (int i = settings->recentProjects().count()-1; i >= 0; i--)
            writeTextElement("Project", settings->recentProjects().at(i));
    }
    writeEndDocument();
    return true;
}

void PlatzWriter::writeSettings()
{
    if (!settings)
        return;
    writeStartElement("ProjectSettings");
    writeStartElement("SliceSize");
    writeAttribute("width", QString::number(settings->sliceSize().width()));
    writeAttribute("height", QString::number(settings->sliceSize().height()));
    writeEndElement();  // SliceSize
    writeStartElement("SpriteSize");
    writeAttribute("width", QString::number(settings->spriteSize().width()));
    writeAttribute("height", QString::number(settings->spriteSize().height()));
    writeEndElement();  // SpriteSize
    writeTextElement("ImageFormat", settings->imageFormat());
    writeTextElement("SlicePath", settings->slicePath());
    writeTextElement("TilePath", settings->tilePath());
    writeTextElement("MapPath", settings->mapPath());
    writeTextElement("AnimPath", settings->animPath());
    writeTextElement("SrcFolder", settings->srcFolder());
    writeTextElement("MakefilePath", settings->makefilePath());
    writeTextElement("HexfilePath", settings->hexfilePath());
    writeTextElement("PlatzfilePath", settings->platzfilePath());
    writeEndElement();  // ProjectSettings
}

void PlatzWriter::writeItem(WorldItem *item)
{
    switch(item->type()) {
        case WorldItem::Slice:
        {
            Slice *slice = static_cast<Slice*>(item);
            writeStartElement("Slice");
            writeAttribute("replica", QString::number((slice->replicaOf())?slice->replicaOf()->row():-1));
            writeAttribute("locked", QString::number(slice->lockedOrdering()?1:0));
            writeTextElement("Title", "Slice");
            writeBounds(item->limitRect());
            foreach(WorldItem *child, *item->children())
                writeItem(child);
            writeEndElement();
            break;
        }
        case WorldItem::OuterProxy:
        {
            writeStartElement("BgOuterProxy");
            writeTextElement("Title", "Outer BGs");
            foreach(WorldItem *child, *item->children())
                writeItem(child);
            writeEndElement();
            break;
        }
        case WorldItem::Outer:
        {
            BgOuter *bgo = static_cast<BgOuter*>(item);
            writeStartElement("BgOuter");
            writeAttribute("flags", QString::number(bgo->flags()));
            writeAttribute("trigIndex", QString::number(bgo->trigger()));
            writeAttribute("trigOrientation", QString::number(bgo->triggerOrientation()));
            writeTextElement("Title", "BgOuter");
            writeBounds(bgo->boundingRect());
            foreach(WorldItem *child, *item->children())
                writeItem(child);
            writeEndElement();
            break;
        }
        case WorldItem::Inner:
        {
            BgInner *bgi = static_cast<BgInner*>(item);
            writeStartElement("BgInner");
            writeAttribute("flags", QString::number(bgi->flags()));
            writeAttribute("tile", QString::number(bgi->tile()));
            writeTextElement("Title", "BgInner");
            writeBounds(bgi->boundingRect());
            writeEndElement();
            break;
        }
        case WorldItem::Mutable:
        {
            BgMutable *bgm = static_cast<BgMutable*>(item);
            writeStartElement("BgMutable");
            writeAttribute("flags", QString::number(bgm->flags()));
            writeAttribute("tile", QString::number(bgm->tile()));
            writeAttribute("mutableString", bgm->mutableString());
            writeAttribute("custom", QString::number((bgm->isCustomPayload()?1:0)));
            writeTextElement("Title", "BgMutable");
            writeBounds(bgm->boundingRect());
            writeMutablePayload(bgm->payload());
            writeEndElement();
            break;
        }
        case WorldItem::ObjectProxy:
        {
            writeStartElement("BgObjectProxy");
            writeTextElement("Title", "BG Objects");
            foreach(WorldItem *child, *item->children())
                writeItem(child);
            writeEndElement();
            break;
        }
        case WorldItem::Object:
        {
            BgObject *obj = static_cast<BgObject*>(item);
            writeStartElement("BgObject");
            writeAttribute("map", QString::number(obj->map()));
            writeTextElement("Title", "BgObject");
            writeBounds(obj->boundingRect());
            writeEndElement();
            break;
        }
        case WorldItem::PlatformPathProxy:
        {
            writeStartElement("PlatformPathProxy");
            writeTextElement("Title", "Moving Platforms");
            foreach(WorldItem *child, *item->children())
                writeItem(child);
            writeEndElement();
            break;
        }
        case WorldItem::PlatformPath:
        {
            BgPlatformPath *platformPath = static_cast<BgPlatformPath*>(item);
            writeStartElement("PlatformPath");
            writeAttribute("axis", QString::number(platformPath->axis()));
            writeTextElement("Title", "PlatformPath");
            writeBounds(platformPath->boundingRect());
            foreach(WorldItem *child, *item->children())
                writeItem(child);
            writeEndElement();
            break;
        }
        case WorldItem::Platform:
        {
            BgPlatform *platform = static_cast<BgPlatform*>(item);
            writeStartElement("Platform");
            writeAttribute("flags", QString::number(platform->flags()));
            writeAttribute("clearTile", platform->clearTile());
            writeAttribute("velocity", QString::number(platform->velocity()));
            writeTextElement("Title", "Platform");
            writeBounds(platform->boundingRect());
            writeEndElement();
            break;
        }
        default:
            break;
    }
}

void PlatzWriter::writeBounds(const QRectF &r, QString title)
{
    writeStartElement(title);
    writeTextElement("Left", QString::number(r.left()));
    writeTextElement("Top", QString::number(r.top()));
    writeTextElement("Right", QString::number(r.right()));
    writeTextElement("Bottom", QString::number(r.bottom()));
    writeEndElement();
}

void PlatzWriter::writeMutablePayload(const Platz::MutablePayload payload, const QString title)
{
    writeStartElement(title);
    writeTextElement("Left", QString::number(payload.left));
    writeTextElement("Right", QString::number(payload.right));
    writeTextElement("Top", QString::number(payload.top));
    writeTextElement("Bottom", QString::number(payload.btm));
    writeEndElement();
}
