
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

#include <QDir>
#include <QFile>
#include <SourceParser.h>
#include <PlatzDataModel.h>
#include <WorldItem.h>
#include <Slice.h>
#include <Level.h>
#include <BgOuter.h>
#include <ProxyItem.h>
#include <BgMutable.h>
#include <BgObject.h>
#include <BgPlatformPath.h>
#include <BgPlatform.h>
#include "PlatzReader.h"

PlatzReader::PlatzReader(Settings *settings, PlatzDataModel *model)
    : settings(settings), model(model), sliceCount(0), prevBgi(0)
{
    if (model)
        worldPtr.push(model->root());
}

PlatzReader::ReadError PlatzReader::loadProject(const QString &path, const ReadType &type)
{
    if (!settings || (type == ReadWorld && !model))
        return ProgramError;

    QFile file(path);

    if (!file.open(QFile::ReadOnly | QFile::Text))
        return FileError;
    setDevice(&file);
    sliceCount = 0;

    ReadError rerr = NoError;

    while(!atEnd()) {
        readNext();

        if (isStartElement()) {
            if (name() == "LePlatz") {
                version = attributes().value("version").toString();

                if (Platz::LEPLATZ_VERSIONS.contains(version)) {
                    readLePlatzProject(type);
                    break;
                } else {
                    rerr = VersionError;
                    break;
                }
            }
            raiseError(QObject::tr("File is not a valid Platz World Save file"));   // Makes atEnd() true
        }
    }
    file.close();

    if (error())
        rerr = InvalidError;
    return rerr;
}


PlatzReader::ReadError PlatzReader::loadLePlatzSettings(const QString &path, QByteArray &winGeometry, QByteArray &winLayout)
{
    if (!settings)
        return ProgramError;

    QFile file(path);

    if (!file.open(QFile::ReadOnly | QFile::Text))
        return FileError;
    setDevice(&file);

    QString s;
    ReadError rerr = NoError;

    while(!atEnd()) {
        readNext();

        if (isStartElement()) {
            s = name().toString();

            if (s == "LePlatz") {
                if (!Platz::LEPLATZ_VERSIONS.contains(attributes().value("version").toString())) {
                    rerr = VersionError;
                    break;
                }
            } else if (s == "LePlatzSettings") {
                ; // Do nothing
            } else if (s == "ScreenLayout") {
                winGeometry = readElementText().toLatin1();
                winGeometry = winGeometry.fromHex(winGeometry);
            } else if (s == "WinLayout") {
                winLayout = readElementText().toLatin1();
                winLayout = winLayout.fromHex(winLayout);
            } else if (s == "CanvasColor") {
                bool ok;
                unsigned int color = readElementText().toUInt(&ok, 16);

                if (ok)
                    settings->setCanvasColor(QColor(QRgb(color)));
            } else if (s == "MakeExePath") {
                settings->setMakeExePath(readElementText());
            } else if (s == "EmuExePath") {
                settings->setEmuExePath(readElementText());
            } else if (s == "UpdatesUrl") {
                settings->setUpdatesUrl(readElementText());
            } else if (s == "RecentProjects") {
                ; // Do nothing
            } else  if (s == "Project") {
                settings->addRecentProject(readElementText());
            } else {
                readElementText();
            }
        }
    }
    file.close();

    if (error())
        rerr = InvalidError;
    return rerr;
}

QString PlatzReader::lookupErrorString(const ReadError &err)
{
    switch (err) {
    case NoError: return ""; break;
    case ProgramError: return "LePlatz program error."; break;
    case FileError: return "File access error."; break;
    case InvalidError: return "Invalid file format."; break;
    case VersionError: return "File version not supported."; break;
    default: return ""; break;
    }
}

void PlatzReader::readLePlatzProject(const ReadType &type)
{
    Q_ASSERT(isStartElement() && name() == "LePlatz");

    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;
        if (isStartElement()) {
            if (name() == "ProjectSettings") {
                readSettings();

                if (version == Platz::LEPLATZ_VERSIONS.first())
                    legacySupport_v1_0(0);
                if (type == ReadSettings)
                    break;
                continue;
            }
            if (type == ReadSettings)   // Settings shouldn't deal with World/Slice
                continue;
            if (name() == "World")
                continue;
            else if (name() == "Slice")
                readSlice();
            else
                readUnknownElement();
        }
    }
}

void PlatzReader::readWindowState(QByteArray &winState)
{
    Q_ASSERT(isStartElement() && name() == "WinState");

    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;
        if (isStartElement()) {
            if (name() == "Layout") {
                winState = readElementText().toLatin1();
                winState = winState.fromHex(winState);
            } else {
                readUnknownElement();
            }
        }
    }
}

void PlatzReader::readSettings()
{
    Q_ASSERT(isStartElement() && name() == "ProjectSettings");
    Q_ASSERT(settings);

    QString s;

    settings->setVideoMode(3);  // v1.0 compatibility
    settings->setOffsetY(0);    //

    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;
        if (isStartElement()) {
            s = name().toString();

            if (s == "VideoMode") {
                int vmode = attributes().value("mode").toString().toInt();
                settings->setVideoMode(vmode);
                readElementText();
            } else if (s == "SliceSize") {
                int width = attributes().value("width").toString().toInt();
                int height = attributes().value("height").toString().toInt();
                settings->setSliceSize(width, height);
                readElementText();
            } else if (s == "OffsetY") {
                settings->setOffsetY(attributes().value("offset").toString().toInt());
                readElementText();
            } else if (s == "SpriteSize") {     // v1.0 remnant
                int width = attributes().value("width").toString().toInt();
                int height = attributes().value("height").toString().toInt();
                settings->setSpriteSize(width, height);
                readElementText();
            } else if (s == "ImageFormat") {    // v1.0 remnant
                settings->setImageFormat(readElementText());
            } else if (s == "SlicePath") {
                settings->setSlicePath(readElementText());
            } else if (s == "TilePath") {
                settings->setTilePath(readElementText());
            } else if (s == "MapPath") {
                settings->setMapPath(readElementText());
            } else if (s == "AnimPath") {
                settings->setAnimPath(readElementText());
            } else if (s == "SrcFolder") {
                settings->setSrcFolder(readElementText());
            } else if (s == "MakefilePath") {
                settings->setMakefilePath(readElementText());
            } else if (s == "HexfilePath") {
                settings->setHexfilePath(readElementText());
            } else if (s == "PlatzfilePath") {
                settings->setPlatzfilePath(readElementText());
            }
        }
    }
}

void PlatzReader::readSlice()
{
    Q_ASSERT(isStartElement() && name() == "Slice");
    Q_ASSERT(!worldPtr.isEmpty());

    int replica = attributes().value("replica").toString().toInt();
    int locked = attributes().value("locked").toString().toInt();
    QString title = readString("Title");
    Slice *slice = new Slice(QList<QVariant>() << title, worldPtr.top());
    model->insertRow(worldPtr.top()->childCount(), slice, model->indexOf(worldPtr.top()->row(), 0, worldPtr.top()));
    slice->setBoundingRect(QRectF(sliceCount*settings->sliceSize().width(),0.0,settings->sliceSize().width(),settings->sliceSize().height()));
    slice->setGraphicalRepresentation(new PlatzGraphicsItem(slice, Platz::INVISIBLE));
    slice->setLockedOrdering(locked);
    worldPtr.push(slice);

    if (replica != -1 && replica < model->root()->childCount())
        slice->setReplica(static_cast<Slice*>(model->root()->child(replica)));

    while (!atEnd()) {
        readNext();

        if (isEndElement()) {
            Q_ASSERT(!worldPtr.isEmpty());
            worldPtr.pop();
            break;
        }

        if (isStartElement()) {
            if (name() == "BgOuterProxy")
                readBgOuterProxy();
            else if (name() == "BgObjectProxy")
                readBgObjectProxy();
            else if (name() == "PlatformPathProxy")
                readPlatformPathProxy();
            else
                readUnknownElement();
        }
    }
    ++sliceCount;
}

void PlatzReader::readBgOuterProxy()
{
    Q_ASSERT(isStartElement() && name() == "BgOuterProxy");
    Q_ASSERT(!worldPtr.isEmpty());

    QString title = readString("Title");
    ProxyItem *proxy = new ProxyItem(QList<QVariant>() << title, WorldItem::OuterProxy, worldPtr.top());
    proxy->setGraphicalRepresentation(new PlatzGraphicsItem(proxy, Platz::INVISIBLE));
    model->insertRow(worldPtr.top()->childCount(), proxy, model->indexOf(worldPtr.top()->row(), 0, worldPtr.top()));
    worldPtr.push(proxy);

    while (!atEnd()) {
        readNext();

        if (isEndElement()) {
            Q_ASSERT(!worldPtr.isEmpty());
            worldPtr.pop();
            break;
        }

        if (isStartElement()) {
            if (name() == "BgOuter")
                readBgOuter();
            else
                readUnknownElement();
        }
    }
}

void PlatzReader::readBgOuter()
{
    Q_ASSERT(isStartElement() && name() == "BgOuter");
    Q_ASSERT(!worldPtr.isEmpty());

    int flags = attributes().value("flags").toString().toInt();
    QString title, trigger, triggerOrientation;

    if ((version == Platz::LEPLATZ_VERSIONS.first()) && (flags&BgOuter::BGT)) {
        int trigIndex = attributes().value("trigIndex").toString().toInt();
        int trigOrientation = attributes().value("trigOrientation").toString().toInt();

        trigger = WorldItem::triggerIdAt(trigIndex);

        if (trigOrientation == 1)
            triggerOrientation = Platz::TRIGGER_NML;
        else if (trigOrientation == -1)
            triggerOrientation = Platz::TRIGGER_REV;
        else
            triggerOrientation = "";
    }
    title = readString("Title");

    if (version != Platz::LEPLATZ_VERSIONS.first() && (flags&BgOuter::BGT)) {
        trigger = readString("Trigger");
        triggerOrientation = readString("TriggerOri");
    }

    BgOuter *bgo = new BgOuter(QList<QVariant>() << title, worldPtr.top());
    bgo->setBoundingRect(readRect("Bounds"));
    bgo->setGraphicalRepresentation(new PlatzGraphicsItem(bgo, Platz::NORMAL));
    bgo->setFlags(flags);
    bgo->setTrigger(trigger);
    bgo->setTriggerOrientation(triggerOrientation);
    model->insertRow(worldPtr.top()->childCount(), bgo, model->indexOf(worldPtr.top()->row(), 0, worldPtr.top()));
    worldPtr.push(bgo);

    while (!atEnd()) {
        readNext();

        if (isEndElement()) {
            Q_ASSERT(!worldPtr.isEmpty());
            worldPtr.pop();
            break;
        }

        if (isStartElement()) {
            if (name() == "BgInner")
                readBgInner();
            else if (name() == "BgMutable")
                readBgMutable();
            else
                readUnknownElement();
        }
    }
}

void PlatzReader::readBgInner()
{
    Q_ASSERT(isStartElement() && name() == "BgInner");
    Q_ASSERT(!worldPtr.isEmpty());

    int flags = attributes().value("flags").toString().toInt();
    int tile = attributes().value("tile").toString().toInt();
    QString title = readString("Title");
    BgInner *bgi = new BgInner(QList<QVariant>() << title, worldPtr.top());

    if (flags&BgInner::BGMC)
        bgi->setBgmClass(readString("Bgmc"));
    bgi->setBoundingRect(readRect("Bounds"));
    bgi->setGraphicalRepresentation(new PlatzGraphicsItem(bgi, Platz::NORMAL));
    bgi->setFlags(flags);
    bgi->setTile(tile);
    prevBgi = bgi;
    model->insertRow(worldPtr.top()->childCount(), bgi, model->indexOf(worldPtr.top()->row(), 0, worldPtr.top()));

    while (!atEnd()) {
        readNext();

        if (isEndElement())
                break;

        if (isStartElement())
            readUnknownElement();
    }
}

void PlatzReader::readBgMutable()
{
    Q_ASSERT(isStartElement() && name() == "BgMutable");
    Q_ASSERT(!worldPtr.isEmpty());

    if (!prevBgi)
        return;
    int flags = attributes().value("flags").toString().toInt();
    int tile = attributes().value("tile").toString().toInt();
    int custom = attributes().value("custom").toString().toInt();
    QString mutableString, title;

    if (version == Platz::LEPLATZ_VERSIONS.first()) {
        mutableString = attributes().value("mutableString").toString();
        title = readString("Title");
    } else {
        title = readString("Title");
        mutableString = readString("MutString");
    }

    BgMutable *bgm = new BgMutable(QList<QVariant>() << title, prevBgi, worldPtr.top());
    bgm->setBoundingRect(readRect("Bounds"));
    bgm->setMutableString(mutableString);

    if (custom)
        bgm->setCustomPayload(readMutablePayload("Payload"));
    else
        bgm->setPayload(readMutablePayload("Payload").toRect());

    bgm->setGraphicalRepresentation(new PlatzGraphicsItem(bgm, Platz::NORMAL));
    bgm->setFlags(flags);
    bgm->setTile(tile);
    prevBgi->setMutator(bgm);
    bgm->setMutator(prevBgi);
    model->insertRow(worldPtr.top()->childCount(), bgm, model->indexOf(worldPtr.top()->row(), 0, worldPtr.top()));

    while (!atEnd()) {
        readNext();

        if (isEndElement())
                break;

        if (isStartElement())
            readUnknownElement();
    }
}

void PlatzReader::readBgObjectProxy()
{
    Q_ASSERT(isStartElement() && name() == "BgObjectProxy");
    Q_ASSERT(!worldPtr.isEmpty());

    QString title = readString("Title");
    ProxyItem *proxy = new ProxyItem(QList<QVariant>() << title, WorldItem::ObjectProxy, worldPtr.top());
    proxy->setGraphicalRepresentation(new PlatzGraphicsItem(proxy, Platz::INVISIBLE));
    model->insertRow(worldPtr.top()->childCount(), proxy, model->indexOf(worldPtr.top()->row(), 0, worldPtr.top()));
    worldPtr.push(proxy);

    while (!atEnd()) {
        readNext();

        if (isEndElement()) {
            Q_ASSERT(!worldPtr.isEmpty());
            worldPtr.pop();
            break;
        }

        if (isStartElement()) {
            if (name() == "BgObject")
                readBgObject();
            else
                readUnknownElement();
        }
    }
}

void PlatzReader::readBgObject()
{
    Q_ASSERT(isStartElement() && name() == "BgObject");
    Q_ASSERT(!worldPtr.isEmpty());

    int map = attributes().value("map").toString().toInt();
    QString title = readString("Title");
    BgObject *obj = new BgObject(QList<QVariant>() << title, worldPtr.top());
    obj->setBoundingRect(readRect("Bounds"));
    obj->setGraphicalRepresentation(new PlatzGraphicsItem(obj, Platz::NORMAL));

    obj->setMap(map);
    model->insertRow(worldPtr.top()->childCount(), obj, model->indexOf(worldPtr.top()->row(), 0, worldPtr.top()));

    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement())
            readUnknownElement();
    }
}

void PlatzReader::readPlatformPathProxy()
{
    Q_ASSERT(isStartElement() && name() == "PlatformPathProxy");
    Q_ASSERT(!worldPtr.isEmpty());

    QString title = readString("Title");
    ProxyItem *proxy = new ProxyItem(QList<QVariant>() << title, WorldItem::PlatformPathProxy, worldPtr.top());
    proxy->setGraphicalRepresentation(new PlatzGraphicsItem(proxy, Platz::INVISIBLE));
    model->insertRow(worldPtr.top()->childCount(), proxy, model->indexOf(worldPtr.top()->row(), 0, worldPtr.top()));
    worldPtr.push(proxy);

    while (!atEnd()) {
        readNext();

        if (isEndElement()) {
            Q_ASSERT(!worldPtr.isEmpty());
            worldPtr.pop();
            break;
        }

        if (isStartElement()) {
            if (name() == "PlatformPath")
                readPlatformPath();
            else
                readUnknownElement();
        }
    }
}

void PlatzReader::readPlatformPath()
{
    Q_ASSERT(isStartElement() && name() == "PlatformPath");
    Q_ASSERT(!worldPtr.isEmpty());

    int axis = attributes().value("axis").toString().toInt();
    QString title = readString("Title");
    BgPlatformPath *platPath = new BgPlatformPath(QList<QVariant>() << title, worldPtr.top());
    platPath->setBoundingRect(readRect("Bounds"));
    platPath->setGraphicalRepresentation(new PlatzGraphicsItem(platPath, Platz::NORMAL));

    platPath->setAxis(static_cast<BgPlatformPath::PlatformAxis>(axis));
    model->insertRow(worldPtr.top()->childCount(), platPath, model->indexOf(worldPtr.top()->row(), 0, worldPtr.top()));
    worldPtr.push(platPath);

    while (!atEnd()) {
        readNext();

        if (isEndElement()) {
            Q_ASSERT(!worldPtr.isEmpty());
            worldPtr.pop();
            break;
        }

        if (isStartElement()) {
            if (name() == "Platform")
                readPlatform();
            else
                readUnknownElement();
        }
    }
}

void PlatzReader::readPlatform()
{
    Q_ASSERT(isStartElement() && name() == "Platform");
    Q_ASSERT(!worldPtr.isEmpty());

    int flags = attributes().value("flags").toString().toInt();
    int vel = attributes().value("velocity").toString().toInt();
    QString clrTile, title;

    if (version == Platz::LEPLATZ_VERSIONS.first()) {
        clrTile = attributes().value("clearTile").toString();
        title = readString("Title");
    } else {
        title = readString("Title");
        clrTile = readString("ClearTile");
    }

    BgPlatform *plat = new BgPlatform(QList<QVariant>() << title, worldPtr.top());
    plat->setBoundingRect(readRect("Bounds"));
    plat->setGraphicalRepresentation(new PlatzGraphicsItem(plat, Platz::NORMAL));
    plat->setFlags(static_cast<BgPlatform::PlatformStyle>(flags));
    plat->setClearTile(clrTile);
    plat->setVelocity(vel);
    model->insertRow(worldPtr.top()->childCount(), plat, model->indexOf(worldPtr.top()->row(), 0, worldPtr.top()));

    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement())
            readUnknownElement();
    }
}

void PlatzReader::readUnknownElement()
{
    Q_ASSERT(isStartElement());

    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement())
            readUnknownElement();
    }
}

QString PlatzReader::readString(const QString &str)
{
    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement()) {
            if (name() == str)
                return(readElementText());
            else
                readUnknownElement();
        }
    }
    return "";
}

QRectF PlatzReader::readRect(const QString &eleName)
{
    QRectF rect(0.0,0.0,0.0,0.0);
    QString s;

    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement()) {
            s = name().toString();
            if (s == eleName)
                continue;
            else if (s == "Left")
                rect.setLeft(readElementText().toDouble());
            else if (s == "Top")
                rect.setTop(readElementText().toDouble());
            else if (s == "Right")
                rect.setRight(readElementText().toDouble());
            else if (s == "Bottom")
                rect.setBottom(readElementText().toDouble());
            else
                readUnknownElement();
        }
    }
    return rect;
}

Platz::MutablePayload PlatzReader::readMutablePayload(const QString &eleName)
{
    Platz::MutablePayload payload;
    QString s;

    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement()) {
            s = name().toString();
            if (s == eleName)
                continue;
            else if (s == "Left")
                payload.left = readElementText().toInt();
            else if (s == "Right")
                payload.right = readElementText().toInt();
            else if (s == "Top")
                payload.top = readElementText().toInt();
            else if (s == "Bottom")
                payload.btm = readElementText().toInt();
            else
                readUnknownElement();
        }
    }
    return payload;
}

void PlatzReader::legacySupport_v1_0(int type)
{
    switch (type) {
        case 0:
            // LePlatz project file format stored src define strings as indexes into loaded src defines. This
            // was intended to allow source code naming updates to reflect within LePlatz, but it broke in
            // cases where the order of defines was rearranged. v1.1 onwards stores the strings as strings
            // and provides a tool for users to update naming changes.
            QString srcFolder = settings->resolvePath(settings->srcFolder());
            QDir dir(srcFolder);

            if (!dir.exists())
                return;
            SourceParser parser;
            WorldItem::triggerIds = parser.parseForStrings(srcFolder, Platz::TRIGGER_IDS_MARKER);
            break;
    }
}


