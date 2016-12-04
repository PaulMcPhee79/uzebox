
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

#ifndef PLATZDATASTREAM_H
#define PLATZDATASTREAM_H

#include <QDataStream>
#include <WorldItem.h>

// May be able to remove this class and use QDataStream operator << (qint64 i)
// and still be safe for 32 or 64 bit systems
class PlatzDataStream : public QDataStream
{
public:
    PlatzDataStream();
    PlatzDataStream(QByteArray *a, QIODevice::OpenMode mode);

    PlatzDataStream& operator<<(WorldItem *w);
    PlatzDataStream& operator>>(WorldItem **w);
};

#endif // PLATZDATASTREAM_H
