
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

#include <QVariant>
#include <QString>
#include <QList>
#include "PlatzDataStream.h"

#undef  CHECK_STREAM_PRECOND
#ifndef QT_NO_DEBUG
#define CHECK_STREAM_PRECOND(retVal, dev) \
    if (!dev) { \
        qWarning("QDataStream: No device"); \
        return retVal; \
    }
#else
#define CHECK_STREAM_PRECOND(retVal, devl) \
    if (!dev) { \
        return retVal; \
    }
#endif

PlatzDataStream::PlatzDataStream()
{
}

PlatzDataStream::PlatzDataStream(QByteArray *a, QIODevice::OpenMode mode)
    : QDataStream(a, mode)
{
}

PlatzDataStream& PlatzDataStream::operator<<(WorldItem *w)
{
    if (w) {
        QIODevice *dev = device();
        CHECK_STREAM_PRECOND(*this, dev)

        dev->write((char*)&w, sizeof(w));
    }
    return *this;
#if 0
        WorldItem *parent = w->parent();
        int childCount = w->childCount();
        WorldItem::WorldItemType type = w->type();
        //QString s(w->data(0).toString());

        dev->write((char*)&parent, sizeof(myParent));
        dev->write((char*)&childCount, sizeof(childCount));
        const QList<WorldItem*>* children = w->children();

        foreach(WorldItem *child, *children)
            dev->write((char*)&child, sizeof(child));

        dev->write((char*)&type, sizeof(type));
        //dev->write((char*)&s, sizeof(s));
#endif
}

PlatzDataStream& PlatzDataStream::operator>>(WorldItem **w)
{
    QIODevice *dev = device();
    CHECK_STREAM_PRECOND(*this, dev)

    if (dev->read((char*)w, sizeof(w)) != sizeof(w)) {
        setStatus(ReadPastEnd);
        *w = 0;
    }
    return *this;
#if 0
    QIODevice *dev = device();
    WorldItem *parent = 0;
    int childCount;
    WorldItem* child = 0;
    WorldItem::WorldItemType type;
    //QString s;

    CHECK_STREAM_PRECOND(*this, dev)

    if (dev->read((char*)&parent, sizeof(parent)) == sizeof(parent)) {
        if (dev->read((char*)&childCount, sizeof(childCount)) == sizeof(childCount)) {
            for (int i = 0; i < childCount; i++) {
                if (dev->read((char*)&child, sizeof(child)) != sizeof(child))
                    break;
                //w->appendChild(child);
            }
            if (dev->read((char*)&type, sizeof(type)) == sizeof(type)) {
                //if (dev->read((char*)&s, sizeof(s)) == sizeof(s)) {
                    w->setParent(parent);
                    w->setType(type);
                    //w->setData(QVariant(s));
                    w->setData(QVariant("Slice X"));
                    return *this;
                //}
            }
        }
    }
    setStatus(ReadPastEnd);
    return *this;
#endif
}
