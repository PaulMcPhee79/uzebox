
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

#include <QtGui/QListWidget>
#include <QResizeEvent>
#include <QPainter>
#include <QWheelEvent>
#include "PlatzGraphicsList.h"

// Modified from original source: http://doc.trolltech.com/4.5/itemviews-puzzle.html
PlatzGraphicsList::PlatzGraphicsList(QWidget *parent)
    : QListWidget(parent), delimiter(QImage()), zoom(false)
{
    setDragEnabled(true);
    setViewMode(QListView::IconMode);
    setIconSize(QSize(32, 32));
    setSpacing(0);
    setAcceptDrops(false);
    setDropIndicatorShown(true);
    queryImage = QImage(":/icons/128x128/qmark.png").scaled(QSize(32,32), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void PlatzGraphicsList::setDelimiterTile(const QImage &img)
{
    delimiter = img;
}

void PlatzGraphicsList::setCurrentImage(int index)
{
    if (index == Platz::CLEAR)
        emit currentImageChanged(emptyImage);
    else if (index == -1)
        emit currentImageChanged(queryImage);
    else if (index < images.count())
        emit currentImageChanged(images.at(index));
}

void PlatzGraphicsList::clearImages()
{
    clear();
    images.clear();
}

void PlatzGraphicsList::imageToMapIndex(WorldItem *item, const QImage &img)
{
    if (!item || (item->type() != WorldItem::Inner && item->type() != WorldItem::Object))
        return;

    int wid, hgt;
    QImage imgSml, imgItSml;

    foreach(QImage imgIt, images) {
        if (img.size() != imgIt.size()) {
            wid = qMin(img.width(),imgIt.width());
            hgt = qMin(img.height(),imgIt.height());
            imgSml = img.copy(0, 0, wid, hgt);
            imgItSml = imgIt.copy(0, 0, wid, hgt);

            if (imgSml == imgItSml) {   // Not always a positive id, but a good guess - user can override
                if (item->type() == WorldItem::Inner)
                    static_cast<BgInner*>(item)->setTile(images.indexOf(imgIt));
                else
                    static_cast<BgObject*>(item)->setMap(images.indexOf(imgIt));
                emit imageMapped(imgIt);
                return;
            }
        } else if (img == imgIt) {
            if (item->type() == WorldItem::Inner)
                static_cast<BgInner*>(item)->setTile(images.indexOf(imgIt));
            else
                static_cast<BgObject*>(item)->setMap(images.indexOf(imgIt));
            emit imageMapped(imgIt);
            return;
        }
    }
    // No match found
    emit imageMapped(queryImage);
}

int PlatzGraphicsList::contains(const QImage &img)
{
    return images.indexOf(img, 0);
}

void PlatzGraphicsList::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Control:
            zoom = true;
            event->accept();
            break;
        default:
            QListWidget::keyPressEvent(event);
            break;
    }
}

void PlatzGraphicsList::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Control:
            zoom = false;
            event->accept();
            break;
        default:
            QListWidget::keyReleaseEvent(event);
            break;
    }
}

void PlatzGraphicsList::wheelEvent(QWheelEvent *event)
{
    if (!zoom) {
        QListWidget::wheelEvent(event);
        return;
    }

    int scale, width, height;

    scale = width = iconSize().width();
    height = iconSize().height();

    if (event->delta() > 0) {   // Zoom in
        if (scale < 128)
            scale <<= 1;
    } else if (scale > 32) {    // Zoom out
        scale >>= 1;
    }

    if (scale != width) {
        setIconSize(QSize(scale, scale));

        for (int i = 0; i < count(); i++)
            item(i)->setIcon(QIcon(QPixmap::fromImage(images.at(i)).scaled(scale,scale,Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    }
    event->accept();
}

void PlatzGraphicsList::populate(int scaleX, int scaleY, int alpha, const QPixmap &p)
{
    QImage nextTile;
    QRect pixBounds;
    QPixmap pixmap(scaleX, scaleY);
    QPainter painter;
    int top = 0, right = 0;
    int delimWid = qMax(delimiter.width(), 1), delimHgt = qMax(delimiter.height(), 1);
    int xIntervals = p.width()/delimWid;
    int yIntervals = p.height()/delimHgt;

    clear();
    images.clear();
    setIconSize(QSize(scaleX, scaleY));

    for (int y = 0; y < yIntervals; y++) {
        for (int x = 0; x < xIntervals; x++, right=x) {
            nextTile = p.copy(x*delimWid, y*delimHgt, delimWid, delimHgt).toImage();

            if (nextTile == delimiter) {
                if (x == 0) {
                    // Map boundary reached
                    if (right && (y-top)) { // Make sure region is not empty
                        pixBounds.setRect(0, top*delimHgt, right*delimWid, (y-top)*delimHgt);
                        createAddPiece(pixBounds, scaleX, scaleY, alpha, p, pixmap, painter);
                    }
                    top = y+1;
                    right = 0;
                    break;
                } else {
                    break;
                }
            }
        }
    }
    // If file does not end with a delimiter row, we need to extract the final entry here
    if (right) {
        pixBounds.setRect(0, top*delimHgt, right*delimWid, (yIntervals-top)*delimHgt);
        createAddPiece(pixBounds, scaleX, scaleY, alpha, p, pixmap, painter);
    }
}

void PlatzGraphicsList::createAddPiece(const QRect &bounds, int scaleX, int scaleY, int alpha, const QPixmap &src, QPixmap &dest, QPainter &painter)
{
    if (bounds.isEmpty() || !bounds.isValid())
        return;
    dest.fill(Qt::transparent);
    painter.begin(&dest);
    painter.drawPixmap(0, 0, src.copy(bounds).scaled(scaleX,scaleY,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.fillRect(dest.rect(), QColor(0, 0, 0, alpha));
    painter.end();
    addPiece(dest, QPoint(count()*scaleX,0));  // Draggable image is transparent; icon is opaque
    item(count()-1)->setIcon(QIcon(src.copy(bounds).scaled(scaleX,scaleY,Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    images.append(src.copy(bounds).toImage());
}

void PlatzGraphicsList::populate(int wid, int hgt, int scaleX, int scaleY, int alpha, const QPixmap &p)
{
    QPixmap pixmap(scaleX, scaleY);
    QPainter painter;
    int xIntervals = p.width()/wid;
    int yIntervals = p.height()/hgt;

    clear();
    images.clear();
    setIconSize(QSize(scaleX, scaleY));

    for (int y = 0; y < yIntervals; y++) {
        for (int x = 0; x < xIntervals; x++) {
            pixmap.fill(Qt::transparent);
            painter.begin(&pixmap);
            painter.drawPixmap(0, 0, p.copy(x*wid, y*hgt, wid, hgt).scaled(scaleX,scaleY,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
            painter.fillRect(pixmap.rect(), QColor(0, 0, 0, alpha));
            painter.end();
            addPiece(pixmap, QPoint(x,y));  // Draggable image is transparent; icon is opaque
            item(count()-1)->setIcon(QIcon(p.copy(x*wid, y*hgt, wid, hgt).scaled(scaleX,scaleY,Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
            images.append(p.copy(x*wid, y*hgt, wid, hgt).toImage());
        }
    }
}

void PlatzGraphicsList::addPiece(QPixmap pixmap, QPoint location)
{
    QListWidgetItem *pieceItem = new QListWidgetItem(this);
    pieceItem->setIcon(QIcon(pixmap));
    pieceItem->setData(Qt::UserRole, QVariant(pixmap));
    pieceItem->setData(Qt::UserRole+1, location);
    pieceItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable
                        | Qt::ItemIsDragEnabled);
}

void PlatzGraphicsList::resizeEvent(QResizeEvent *event)
{
    if ((abs(resizeWid-event->size().width()) > 8) || (abs(resizeHgt-event->size().width()) > 8)) {
        resizeWid = event->size().width();
        resizeHgt = event->size().height();
        insertItem(0, takeItem(0)); // FIXME: Replace this hack with some kind of refresh/update/invalidate
    }
    QListWidget::resizeEvent(event);
}

void PlatzGraphicsList::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("image/platzmap"))
        event->accept();
    else
        event->ignore();
}

void PlatzGraphicsList::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("image/platzmap")) {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    } else
        event->ignore();
}

void PlatzGraphicsList::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("image/platzmap")) {
        QByteArray pieceData = event->mimeData()->data("image/platzmap");
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        QPixmap pixmap;
        QPoint location;
        dataStream >> pixmap >> location;

        addPiece(pixmap, location);

        event->setDropAction(Qt::CopyAction);
        event->accept();
    } else
        event->ignore();
}

void PlatzGraphicsList::startDrag(Qt::DropActions /*supportedActions*/)
{
    QListWidgetItem *currItem = currentItem();

    if (!currItem)
        return;

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << type();
    dataStream << row(currItem);

    QPixmap pixmap = qVariantValue<QPixmap>(currItem->data(Qt::UserRole));
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("image/platzmap", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
    drag->setPixmap(pixmap);
    drag->exec(Qt::CopyAction);
}
