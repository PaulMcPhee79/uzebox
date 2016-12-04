
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

#ifndef PLATZGRAPHICSVIEW_H
#define PLATZGRAPHICSVIEW_H

#include <QtGui/QGraphicsView>
#include <QCursor>
#include <PlatzDataModel.h>
#include <PlatzEnums.h>
#include <BgInner.h>
#include "PlatzGraphicsScene.h"

using namespace Platz;

class PlatzGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    PlatzGraphicsView(QWidget *parent = 0);
    ~PlatzGraphicsView();
    void clearBackground();
    bool loadBackground(QString bgImagePath);
    void setModel(PlatzDataModel *model);
    PlatzDataModel* model();
    int currentSlice();
    QSize sliceSize() { return mSliceSize; }
    int sliceIndex() { return mSliceIndex; }
signals:
    void tileDrawn(WorldItem *item, const QImage &img);   // Temporary until we have QListModel subclassed
    void mapDrawn(WorldItem *item, const QImage &img);    // Temporary until we have QListModel subclassed
    void animDrawn(WorldItem *item, const QImage &img);   // Temporary until we have QListModel subclassed
    void mouseCoordsChanged(const QString &coords);
    void sliceIndexChanged(const QString &index);
    void receivedDrop(BgInner::BgiFlags type, int index);
public slots:
    void itemInserted(const QModelIndex &parent, int start, int end);
    void itemDrawn(WorldItem *item);
    void zoomView(int delta);
    void nextSlice();
    void previousSlice();
    InteractionMode mode() { return imode; }
    void setMode(const InteractionMode &mode);
    void renderBackbuffer();
    int sliceCount() { return bgpixList.count(); }
    void removeSlice(QPoint pos);
    void removeSlice(int index);
    void appendSlice();
    void insertSlice(QPoint pos, int beforeOrAfter);
    void insertSlice(int index, int beforeOrAfter, bool draw = true);
    void setSliceSize(const QSize &size);
    void setTileSize(const QSize &size) { mTileSize = size; }
    void updateSliceIndex();
    void deleteSelected();
    void synchronizeModelView();
    void frameItem(const QModelIndex &index);
    void setCanvasColor(const QColor &color) { setBackgroundBrush(QBrush(color)); }
protected:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void scaleView(qreal scaleFactor);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawEmptySlice(int index);
private:
    void enforceCursorRules();
    PlatzGraphicsScene *platzScene;
    QPixmap *bgpix;
    QList<QPixmap> bgpixList;
    PlatzDataModel *dataModel;
    QSize mSliceSize;
    int mSliceIndex;
    QSize mTileSize;
    QList<QCursor*> cursors;
    const QCursor *cursorCurrent;
    QPoint mousePos;
    bool zoom;
    InteractionMode imode;
};

#endif // PLATZGRAPHICSVIEW_H
