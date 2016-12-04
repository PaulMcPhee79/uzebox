
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

    enum ReadError {
        NoError = 0x00,
        ProgramError = 0x01,
        FileError = 0x02,
        InvalidError = 0x04,
        VersionError = 0x08
    };

    PlatzReader(Settings *settings, PlatzDataModel *model = 0);
    PlatzReader();
    PlatzReader::ReadError loadProject(const QString &path, const ReadType &type);
    PlatzReader::ReadError loadLePlatzSettings(const QString &path, QByteArray &winGeometry, QByteArray &winLayout);
    QString lookupErrorString(const ReadError &err);
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
    void legacySupport_v1_0(int type);
    QString readString(const QString &str);
    QRectF readRect(const QString &eleName);
    Platz::MutablePayload readMutablePayload(const QString &eleName);
    Settings *settings;
    PlatzDataModel *model;
    QStack<WorldItem*> worldPtr;
    int sliceCount;
    BgInner *prevBgi;
    QString version;
};

#endif // PLATZREADER_H
