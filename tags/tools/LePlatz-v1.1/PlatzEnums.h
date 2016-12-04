
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

#ifndef PLATZENUMS_H
#define PLATZENUMS_H

#include <QRect>
#include <QStringList>

namespace Platz
{
    typedef struct MutablePayload {
        int left;
        int right;
        int top;
        int btm;

        MutablePayload()
                : left(0), right(0), top(0), btm(0) {}


        QRectF fromRectF(const QRect &rect) {
            left = (int)rect.left();
            right = (int)rect.left()+rect.width();
            top = (int)rect.top();
            btm = (int)rect.top()+rect.height();
            return toRectF();
        }

        QRect fromRect(const QRect &rect) {
            left = rect.left();
            right = rect.left()+rect.width();
            top = rect.top();
            btm = rect.top()+rect.height();
            return toRect();
        }

        QRect toRect() const {
            return QRect(left, top, right-left, btm-top);
        }

        QRectF toRectF() const {
            return QRectF(toRect());
        }

        QString toString(int yOffset, int divX = 1, int divY = 1) const {
            if (divX <= 0 || divY <= 0)
                return "MutablePayload::toString - invalid parameters.";
            return "{" + QString::number(left / divX) + "," + QString::number(right / divX) + "," +
                QString::number((top+yOffset) / divY) + "," + QString::number((btm+yOffset) / divY) + "}";
        }
    } MutablePayload;

    typedef struct NumericDefine {
        QString name;
        int value;
    } NumericDefine;

    enum InteractionMode {
        IM_SELECT,
        IM_OUTER_BG,
        IM_INNER_BG,
        IM_OBJECT_BG,
        IM_PLATFORM_PATH,
        IM_PLATFORM,
        IM_MUTABLE_BG,
        IM_INSERT,
        IM_REMOVE,
        IM_REPLICATE
    };

    enum ItemDisplayMode {
        NORMAL,
        SELECTED,
        INVISIBLE
    };

    // File formats
    const QString SUPPORTED_IMAGE_FORMATS_STR = "LePlatz Image Formats (*.bmp *.png)";
    const QString LEPLATZ_SETTINGS_FORMATS_STR = "LePlatz Settings Files (*.xml)";
    const QString PLATZ_INCLUDE_FORMATS_STR = "Platz Include Files (*.inc)";
    const QString SUPPORTED_EXE_FORMATS_STR = "Executable Files (*)";
    const QString INTEL_HEX_FILES_STR = "Intel Hex Files (*.hex)";
    const QString UNDEFINED = "<LEPLATZ_UNDEFINED>";
    const QStringList SUPPORTED_IMAGE_FORMATS(QStringList() << "*.bmp" << "*.png");
    const QStringList SUPPORTED_IMAGE_FORMATS_EXTS(QStringList() << ".bmp" << ".png");
    const QString MUTABLE_IDS_MARKER = "_LePlatz_Mutable_Bg_Ids_";
    const QString MUTABLE_CLASS_IDS_MARKER = "_LePlatz_Mutable_Class_Ids_";
    const QString TRIGGER_IDS_MARKER = "_LePlatz_Trigger_Ids_";
    const QString PLATFORM_CLR_TILE_IDS_MARKER = "_LePlatz_Platform_Clear_Tiles_Ids_";
    const QString TRIGGER_NML = "ORI_LRUD";
    const QString TRIGGER_REV = "ORI_RLDU";
    const QStringList LEPLATZ_VERSIONS(QStringList() << "1.0" << "1.1");
    const int MAX_RECENT_PROJECTS = 8;
    const int CLEAR = 65535;
}

#endif // PLATZENUMS_H
