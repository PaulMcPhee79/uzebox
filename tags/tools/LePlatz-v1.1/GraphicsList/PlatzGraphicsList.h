
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

#ifndef PLATZGRAPHICSLIST_H
#define PLATZGRAPHICSLIST_H

#include <QListWidget>
#include <BgInner.h>
#include <BgObject.h>

class PlatzGraphicsList : public QListWidget
{
    Q_OBJECT

public:

    PlatzGraphicsList(QWidget *parent);
    void populate(int scaleX, int scaleY, int alpha, const QPixmap &p);
    void populate(int wid, int hgt, int scaleX, int scaleY, int alpha, const QPixmap &p);
    void addPiece(QPixmap pixmap, QPoint location);
    int contains(const QImage &img);
    BgInner::BgiFlags type() { return bgiType; }
signals:
    void imageMapped(const QImage &img);
    void currentImageChanged(const QImage &img);
public slots:
    void imageToMapIndex(WorldItem *item, const QImage &img);
    void setDelimiterTile(const QImage &img);
    void setCurrentImage(int index);
    void clearImages();
    void setType(BgInner::BgiFlags t) { bgiType = t; }
protected:
    void createAddPiece(const QRect &bounds, int scaleX, int scaleY, int alpha, const QPixmap &src, QPixmap &dest, QPainter &painter);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void startDrag(Qt::DropActions supportedActions);
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
private:
    QList<QImage> images;  // Could hash this if comparisons run slow
    QImage queryImage;
    QImage emptyImage;
    QImage delimiter;
    int resizeWid;
    int resizeHgt;
    bool zoom;
    BgInner::BgiFlags bgiType;
};

#endif // PLATZGRAPHICSLIST_H
