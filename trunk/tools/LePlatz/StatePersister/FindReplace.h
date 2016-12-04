
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

#ifndef FINDREPLACE_H
#define FINDREPLACE_H

#include <QDialog>
#include <PlatzDataModel.h>
#include <WorldItem.h>

namespace Ui {
    class FindReplace;
}

class FindReplace : public QDialog
{
    Q_OBJECT

public:
    FindReplace(PlatzDataModel *model, QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    ~FindReplace();
private slots:
    void replace();
    void updateReplaceThisCombo(int index);
    void updateReplaceWithCombo(int index);
    void populateReplaceThisCombo(const WorldItem::StringDataType &type);
    void populateReplaceWithCombo(const WorldItem::StringDataType &type);
private:
    Ui::FindReplace *ui;
    PlatzDataModel *model;
};

#endif // FINDREPLACE_H
