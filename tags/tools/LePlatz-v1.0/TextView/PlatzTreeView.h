
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

#ifndef PLATZTREEVIEW_H
#define PLATZTREEVIEW_H

#include <QTreeView>
#include <QDropEvent>

//#define PLATZ_MODEL_DEBUG

class PlatzTreeView : public QTreeView
{
    Q_OBJECT
public:
    PlatzTreeView(QWidget *parent = 0);
signals:
    void selectedIndexChanged(const QModelIndex &index);
protected:
    void startDrag (Qt::DropActions supportedActions);
    void keyPressEvent(QKeyEvent *event);
protected slots:
#ifdef PLATZ_MODEL_DEBUG
    void rowsInserted (const QModelIndex &parent, int start, int end);
#endif
};

#endif // PLATZTREEVIEW_H
