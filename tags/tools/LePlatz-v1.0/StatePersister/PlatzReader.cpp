
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

bool PlatzReader::loadProject(const QString &path, const ReadType &type)
{
    if (!settings || (type == ReadWorld && !model))
        return false;

    QFile file(path);

    if (!file.open(QFile::ReadOnly | QFile::Text))
        return false;
    setDevice(&file);
    sliceCount = 0;

    while(!atEnd()) {
        readNext();

        if (isStartElement()) {
            if (name() == "LePlatz" && attributes().value("version") == "1.0") {
                readLePlatzProject(type);
                break;
            } else {
                raiseError(QObject::tr("File is not a valid Platz version 1.0 World Save file"));   // Makes atEnd() true
            }
        }
    }
    file.close();
    return !error();
}


bool PlatzReader::loadLePlatzSettings(const QString &path, QByteArray &winGeometry, QByteArray &winLayout)
{
    if (!settings)
        return false;

    QFile file(path);

    if (!file.open(QFile::ReadOnly | QFile::Text))
        return false;
    setDevice(&file);

    int i = 0;
    bool ok = true;

    while(!atEnd()) {
        readNext();

        if (isStartElement()) {
            switch (i++) {
                case Version:
                    ok = name() == "LePlatz" && attributes().value("version") == "1.0";
                    break;
                case ScrnLayout:
                    winGeometry = readElementText().toLatin1();
                    winGeometry = winGeometry.fromHex(winGeometry);
                    ok = true;
                    break;
                case WinLayout:
                    winLayout = readElementText().toLatin1();
                    winLayout = winLayout.fromHex(winLayout);
                    ok = true;
                    break;
                case LePlatzSettings:
                    ok = name() == "LePlatzSettings";
                    break;
                case MakeExePath:
                    if ((ok = name() == "MakeExePath"))
                        settings->setMakeExePath(readElementText());
                    break;
                case EmuExePath:
                    if ((ok = name() == "EmuExePath"))
                        settings->setEmuExePath(readElementText());
                    break;
                case RecentProjects:
                    ok = name() == "RecentProjects";
                    break;
                default:
                    if (i > RecentProjects)
                        if ((ok = name() == "Project"))
                            settings->addRecentProject(readElementText());
                    break;
            }

            if (!ok)
                return false;
        }
    }
    file.close();
    return !error();
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

    bool ok;
    QString s;

    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;
        if (isStartElement()) {
            s = name().toString();

            if (s == "SliceSize") {
                int width = attributes().value("width").toString().toInt(&ok, 10);
                int height = attributes().value("height").toString().toInt(&ok, 10);
                settings->setSliceSize(width, height);
                readElementText();
            } else if (s == "SpriteSize") {
                int width = attributes().value("width").toString().toInt(&ok, 10);
                int height = attributes().value("height").toString().toInt(&ok, 10);
                settings->setSpriteSize(width, height);
                readElementText();
            } else if (s == "ImageFormat") {
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

    bool ok;
    int replica = attributes().value("replica").toString().toInt(&ok, 10);
    int locked = attributes().value("locked").toString().toInt(&ok, 10);
    QString title = readTitle();
    Slice *slice = new Slice(QList<QVariant>() << title, worldPtr.top());
    model->insertRow(worldPtr.top()->childCount(), slice, model->indexOf(worldPtr.top()->row(), 0, worldPtr.top()));
    slice->setBoundingRect(QRectF(sliceCount<<8,0.0,256,200.0));
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

    QString title = readTitle();
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

    bool ok;
    int flags = attributes().value("flags").toString().toInt(&ok, 10);
    int trigIndex = attributes().value("trigIndex").toString().toInt(&ok, 10);
    int trigOrientation = attributes().value("trigOrientation").toString().toInt(&ok, 10);
    QString title = readTitle();
    BgOuter *bgo = new BgOuter(QList<QVariant>() << title, worldPtr.top());
    bgo->setBoundingRect(readRect("Bounds"));
    bgo->setGraphicalRepresentation(new PlatzGraphicsItem(bgo, Platz::NORMAL));

    bgo->setFlags(flags);
    bgo->setTrigger(trigIndex);
    bgo->setTriggerOrientation(trigOrientation);
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

    bool ok;
    int flags = attributes().value("flags").toString().toInt(&ok, 10);
    int tile = attributes().value("tile").toString().toInt(&ok, 10);
    QString title = readTitle();
    BgInner *bgi = new BgInner(QList<QVariant>() << title, worldPtr.top());
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
    bool ok;
    int flags = attributes().value("flags").toString().toInt(&ok, 10);
    int tile = attributes().value("tile").toString().toInt(&ok, 10);
    int custom = attributes().value("custom").toString().toInt(&ok, 10);
    QString mstr = attributes().value("mutableString").toString();
    QString title = readTitle();
    BgMutable *bgm = new BgMutable(QList<QVariant>() << title, prevBgi, worldPtr.top());
    bgm->setBoundingRect(readRect("Bounds"));

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

    QString title = readTitle();
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

    bool ok;
    int map = attributes().value("map").toString().toInt(&ok, 10);
    QString title = readTitle();
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

    QString title = readTitle();
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

    bool ok;
    int axis = attributes().value("axis").toString().toInt(&ok, 10);
    QString title = readTitle();
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

    bool ok;
    int flags = attributes().value("flags").toString().toInt(&ok, 10);
    QString clrTile = attributes().value("clearTile").toString();
    int vel = attributes().value("velocity").toString().toInt(&ok, 10);
    QString title = readTitle();
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

QString PlatzReader::readTitle()
{
    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement()) {
            if (name() == "Title")
                return(readElementText());
            else
                readUnknownElement();
        }
    }
    return "";
}

QRectF PlatzReader::readRect(const QString &eleName)
{
    bool ok;
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
                rect.setLeft(readElementText().toDouble(&ok));
            else if (s == "Top")
                rect.setTop(readElementText().toDouble(&ok));
            else if (s == "Right")
                rect.setRight(readElementText().toDouble(&ok));
            else if (s == "Bottom")
                rect.setBottom(readElementText().toDouble(&ok));
            else
                readUnknownElement();
        }
    }
    return rect;
}

Platz::MutablePayload PlatzReader::readMutablePayload(const QString &eleName)
{
    bool ok;
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
                payload.left = readElementText().toInt(&ok);
            else if (s == "Right")
                payload.right = readElementText().toInt(&ok);
            else if (s == "Top")
                payload.top = readElementText().toInt(&ok);
            else if (s == "Bottom")
                payload.btm = readElementText().toInt(&ok);
            else
                readUnknownElement();
        }
    }
    return payload;
}


