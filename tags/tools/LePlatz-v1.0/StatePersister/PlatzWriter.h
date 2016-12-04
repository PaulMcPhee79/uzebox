
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

#ifndef PLATZWRITER_H
#define PLATZWRITER_H

#include <QXmlStreamWriter>
#include <Settings.h>

class PlatzWriter : public QXmlStreamWriter
{
public:
    PlatzWriter(Settings *settings, WorldItem *root = 0);
    bool saveWorld(const QString &path);
    bool saveLePlatzSettings(const QString &path, const QByteArray &winGeometry, const QByteArray &winLayout);
private:
    void writeSettings();
    void writeItem(WorldItem *item);
    void writeBounds(const QRectF &r, QString title = "Bounds");
    void writeMutablePayload(const Platz::MutablePayload payload, const QString title = "Payload");
    Settings *settings;
    WorldItem *root;
};

#endif // PLATZWRITER_H
