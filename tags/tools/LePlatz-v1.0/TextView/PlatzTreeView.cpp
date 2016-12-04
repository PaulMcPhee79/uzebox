
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

#include <PlatzDataModel.h>
#include <WorldItem.h>
#include "PlatzTreeView.h"

PlatzTreeView::PlatzTreeView(QWidget *parent)
    : QTreeView(parent)
{
    connect(this, SIGNAL(clicked(QModelIndex)), this, SIGNAL(selectedIndexChanged(QModelIndex)));
    setEditTriggers(NoEditTriggers);
}

// There must be an easier way to cancel a drop based on the recipient node
void PlatzTreeView::startDrag (Qt::DropActions supportedActions)
{
    PlatzDataModel *m = static_cast<PlatzDataModel*>(this->model());
    m->setDropState(PlatzDataModel::Invalid);
    QTreeView::startDrag(supportedActions);
}

void PlatzTreeView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Delete:
        {
            if (selectedIndexes().count() == 0)
                break;
            Q_ASSERT(model());
            QModelIndex index(selectedIndexes().at(0));
            PlatzDataModel *platzModel = static_cast<PlatzDataModel*>(model());
            WorldItem *w = platzModel->instanceOf(index);

            if (w) {
                if (w->type()&(WorldItem::Level|WorldItem::Slice|WorldItem::OuterProxy|WorldItem::ObjectProxy|WorldItem::PlatformPathProxy))
                    break;
                platzModel->setDropState(PlatzDataModel::Valid);
                platzModel->removeRow(w->row(), platzModel->indexOf(w->parent()->row(), 0, w->parent()));
            }
            break;
        }
        case Qt::Key_Enter:
        case Qt::Key_Return:
        {
            if (selectedIndexes().count() == 0)
                break;
            QModelIndex index = selectedIndexes()[0];

            if (!isExpanded(index))
                expand(index);
            else
                collapse(index);
            emit selectedIndexChanged(index);
            break;
        }
    }
    QTreeView::keyPressEvent(event);
}

#ifdef PLATZ_MODEL_DEBUG
void PlatzTreeView::rowsInserted (const QModelIndex &parent, int start, int end)
{
    WorldItem *w = static_cast<WorldItem*>(parent.internalPointer());
    int row = w->row();
    QTreeView::rowsInserted(parent, start, end);
}
#endif
