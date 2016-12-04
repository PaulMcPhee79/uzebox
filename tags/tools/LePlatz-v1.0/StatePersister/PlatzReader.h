
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

#ifndef PLATZREADER_H
#define PLATZREADER_H

#include <QStack>
#include <QString>
#include <QXmlStreamReader>
#include <Settings.h>
#include <BgInner.h>

class PlatzReader : public QXmlStreamReader
{
public:
    enum ReadType {
        ReadSettings,
        ReadWorld
    };

    enum LePlatzSettingsXmlOrder {
        Version,
        LePlatzSettings,
        ScrnLayout,
        WinLayout,
        MakeExePath,
        EmuExePath,
        RecentProjects
    };

    PlatzReader(Settings *settings, PlatzDataModel *model = 0);
    PlatzReader();
    bool loadProject(const QString &path, const ReadType &type);
    bool loadLePlatzSettings(const QString &path, QByteArray &winGeometry, QByteArray &winLayout);
private:
    void readLePlatzProject(const ReadType &type);
    void readSettings();
    void readWindowState(QByteArray &winState);
    void readSlice();
    void readBgOuterProxy();
    void readBgOuter();
    void readBgInner();
    void readBgMutable();
    void readBgObjectProxy();
    void readBgObject();
    void readPlatformPathProxy();
    void readPlatformPath();
    void readPlatform();
    void readUnknownElement();
    QString readTitle();
    QRectF readRect(const QString &eleName);
    Platz::MutablePayload readMutablePayload(const QString &eleName);
    Settings *settings;
    PlatzDataModel *model;
    QStack<WorldItem*> worldPtr;
    int sliceCount;
    BgInner *prevBgi;
};

#endif // PLATZREADER_H
