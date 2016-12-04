
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

#include <QRegExp>
#include <QFileInfoList>
#include <QDir>
#include <QTextStream>
#include <QStringList>
#include "SourceParser.h"

SourceParser::SourceParser()
{
}

QStringList SourceParser::parseForStrings(const QString &path, const QString &tokenId)
{
    QStringList s;
    QList<Platz::NumericDefine> temp = parseForNumerals(path, tokenId);

    foreach (Platz::NumericDefine ndef, temp)
        s.append(ndef.name);
    return s;
}

QList<Platz::NumericDefine> SourceParser::parseForNumerals(const QString &path, const QString &tokenId)
{
    QDir dir(path);
    QFileInfoList fil = dir.entryInfoList(QStringList() << "*.c" << "*.h", QDir::Files|QDir::Readable, QDir::Name);

    mutableIds.clear();

    if (fil.length() == 0)
        return mutableIds;
    QFile *file = 0;

    foreach (QFileInfo fi, fil) {
        file = new QFile(fi.absoluteFilePath());

        if (file->open(QFile::ReadOnly | QFile::Text))
            appendParsedNumerals(file, tokenId, mutableIds);
        file->close();
        delete file;
        file = 0;
    }
    return mutableIds;
}

void SourceParser::appendParsedNumerals(QIODevice *device, const QString &tokenId, QList<Platz::NumericDefine> &ids)
{
    if (!device || !device->isOpen() || !device->isReadable())
        return;
    bool mutableSection = false, commented = false, ok;
    Platz::NumericDefine id;
    QRegExp regexp("[\\s]+");   // To split on whitespace
    QString line;
    QStringList tokens;
    QTextStream ts(device);

    while(!ts.atEnd()) {
        line = ts.readLine();
        updateCommentState(line, commented);

        if (commented)
            continue;
        if (line.contains(tokenId, Qt::CaseInsensitive)) {
            mutableSection = !mutableSection;
            continue;
        }
        if (!mutableSection)
            continue;
        tokens.clear();
        tokens = line.split(regexp, QString::SkipEmptyParts);

        if (tokens.length() < 3 || tokens.at(0) != "#define")
            continue;
        id.name = tokens.at(1);
        id.value = tokens.at(2).toInt(&ok, 10);

        if (!ok)
            id.value = tokens.at(2).toInt(&ok, 16);
        if (ok)
            ids.append(id);
    }
}

void SourceParser::updateCommentState(const QString &line, bool &state)
{
    bool openComment = false, closeComment = false;

    openComment = line.contains("/*");
    closeComment = line.contains("*/");

    if (openComment && closeComment) {
        if (line.lastIndexOf("/*") > line.lastIndexOf("*/"))
            state = true;
        else
            state = false;
    } else if (openComment) {
        state = true;
    } else if (closeComment) {
        state = false;
    }
}
