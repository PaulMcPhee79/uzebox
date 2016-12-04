
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

#include <QWheelEvent>
#include <QKeyEvent>
#include <QRectF>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <math.h>
#include <WorldItem.h>
#include <Slice.h>
#include <ProxyItem.h>
#include <BgInner.h>
#include <BgObject.h>
#include "PlatzGraphicsView.h"

PlatzGraphicsView::PlatzGraphicsView(QWidget *parent)
    : QGraphicsView(parent), bgpix(0), dataModel(0), mTileSize(QSize(8,8)), mousePos(0,0),
        zoom(false), imode(Platz::IM_SELECT)
{
    platzScene = new PlatzGraphicsScene(this);
    setScene(platzScene);
    setSliceSize(QSize(256,224));   // After scene creation so we also set scene's slice size
    setAcceptDrops(true);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);
    setWindowTitle(tr("Level Viewer"));
    cursors.append(new QCursor(QPixmap(":/cursors/select.png"), 0, 0));
    cursors.append(new QCursor(QPixmap(":/cursors/bgo.png"), 0, 0));
    cursors.append(new QCursor(QPixmap(":/cursors/bgi.png"), 0, 0));
    cursors.append(new QCursor(QPixmap(":/cursors/object.png"), 0, 0));
    cursors.append(new QCursor(QPixmap(":/cursors/mpPath.png"), 0, 0));
    cursors.append(new QCursor(QPixmap(":/cursors/mp.png"), 0, 0));
    cursors.append(new QCursor(QPixmap(":/cursors/bgm.png"), 0, 0));
    cursors.append(new QCursor(QPixmap(":/cursors/insert.png"), 0, 0));
    cursors.append(new QCursor(QPixmap(":/cursors/remove.png"), 0 ,0));
    cursors.append(new QCursor(QPixmap(":/cursors/slice.png"), 0 ,0));
    cursorCurrent = cursors.at(Platz::IM_SELECT);
    setBackgroundBrush(QBrush(qRgb(236,233,216)));
    setCacheMode(CacheBackground);
    connect(platzScene, SIGNAL(itemDrawn(WorldItem*)), this, SLOT(itemDrawn(WorldItem*)));
    setMouseTracking(true);
}

void PlatzGraphicsView::setSliceSize(const QSize &size)
{
    mSliceSize = size;

    if (platzScene)
        platzScene->setSliceSize(size);
}

void PlatzGraphicsView::frameItem(const QModelIndex &index)
{
    WorldItem *w = model()->instanceOf(index);

    if (!w)
        return;
    if (w->graphicalRepresentation())
        ensureVisible(w->graphicalRepresentation(), 25, 25);
}

void PlatzGraphicsView::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("image/platzmap")) {
        int type, index;
        QByteArray encodedData = event->mimeData()->data("image/platzmap");
        QDataStream stream(&encodedData, QIODevice::ReadOnly);

        if (!stream.atEnd()) {
            stream >> type;

            if (!stream.atEnd()) {
                stream >> index;

                WorldItem *parent = 0;
                QList<QGraphicsItem*> dropZone;
                dropZone.append(platzScene->items(mapToScene(event->pos())));

                foreach(QGraphicsItem *d, dropZone) {
                    parent = static_cast<PlatzGraphicsItem*>(d)->parent();

                    if (parent) {
                        if (parent->type() == WorldItem::Inner) {
                            BgInner *bgi = static_cast<BgInner*>(parent);

                            if ((bgi->flags()&type) || (((bgi->flags()&(BgInner::BGA|BgInner::BGP)) == 0) && type == BgInner::BG0)) {
                                bgi->setTile(index);
                                emit receivedDrop(static_cast<BgInner::BgiFlags>(type), index);
                                platzScene->emitIndexChanges(parent);
                                break;
                            }
                        } else if (parent->type() == WorldItem::Object && type == BgInner::BGP) {
                            static_cast<BgObject*>(parent)->setMap(index);
                            emit receivedDrop(static_cast<BgInner::BgiFlags>(type), index);
                            platzScene->emitIndexChanges(parent);
                            break;
                        }
                    }
                }
            }
        }
        event->setDropAction(Qt::CopyAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void PlatzGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("image/platzmap")) {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    } else
        event->ignore();
}

void PlatzGraphicsView::itemInserted(const QModelIndex &parent, int start, int end)
{
    if (!parent.isValid())
        return;
    WorldItem *w = dataModel->instanceOf(parent), *child = 0;

    for (int i = start; i <= end; i++) {
        child = w->child(i);

        if (child && !(child->type()&(WorldItem::Level|WorldItem::Slice|WorldItem::OuterProxy|WorldItem::ObjectProxy|WorldItem::PlatformPathProxy))) {
            PlatzGraphicsItem *item = child->graphicalRepresentation();

            if (item) {
                platzScene->addItem(item);

                if (item->mode() == Platz::SELECTED)
                    platzScene->appendSelection(item);
            }
        }
    }

    // Needed for drag 'n' drops
    platzScene->update();
}

int PlatzGraphicsView::currentSlice()
{
    return (int)mapToScene(rect().topLeft()).x()/bgpixList.at(0).width(); // Change this when we have a manager class
}

void PlatzGraphicsView::clearBackground()
{
    delete bgpix;
    bgpix = 0;
    bgpixList.clear();

    if (platzScene)
        platzScene->setSceneRect(0, 0, 0, 0);
}

void PlatzGraphicsView::appendSlice()
{
    insertSlice(bgpixList.count()-1, 1);
}

void PlatzGraphicsView::insertSlice(QPoint pos, int beforeOrAfter)
{
    insertSlice((int)mapToScene(pos).x()/bgpixList.at(0).width(), beforeOrAfter);
}

void PlatzGraphicsView::insertSlice(int index, int beforeOrAfter, bool draw)
{
    Q_ASSERT(platzScene && model() && model()->root());

    if (beforeOrAfter > 0)
        ++index;
    if (index < 0 || index > model()->root()->childCount())
        return;
    int row = 0;
    Slice *slice = 0;
    ProxyItem *bgoProxy = 0, *objProxy = 0, *platPathProxy = 0;

    if ((slice = new Slice(QList<QVariant>() << "Slice", model()->root()))) {
        model()->insertRow(index, slice, model()->indexOf(model()->root()->row(), 0, model()->root()));
        slice->setBoundingRect(QRectF(index*sliceSize().width(),0.0,mSliceSize.width(),mSliceSize.height()));
        slice->setGraphicalRepresentation(new PlatzGraphicsItem(slice, Platz::INVISIBLE));

        if ((bgoProxy = new ProxyItem(QList<QVariant>() << WorldItem::baseData(WorldItem::OuterProxy), WorldItem::OuterProxy, slice))) {
            model()->insertRow(row++, bgoProxy, model()->indexOf(slice->row(), 0, slice));
            bgoProxy->setBoundingRect(slice->boundingRect());
            bgoProxy->setGraphicalRepresentation(new PlatzGraphicsItem(slice, Platz::INVISIBLE));
        }

        if ((objProxy = new ProxyItem(QList<QVariant>() << WorldItem::baseData(WorldItem::ObjectProxy), WorldItem::ObjectProxy, slice))) {
            model()->insertRow(row++, objProxy, model()->indexOf(slice->row(), 0, slice));
            objProxy->setBoundingRect(slice->boundingRect());
            objProxy->setGraphicalRepresentation(new PlatzGraphicsItem(slice, Platz::INVISIBLE));
        }

        if ((platPathProxy = new ProxyItem(QList<QVariant>() << WorldItem::baseData(WorldItem::PlatformPathProxy), WorldItem::PlatformPathProxy, slice))) {
            model()->insertRow(row, platPathProxy, model()->indexOf(slice->row(), 0, slice));
            platPathProxy->setBoundingRect(slice->boundingRect());
            platPathProxy->setGraphicalRepresentation(new PlatzGraphicsItem(slice, Platz::INVISIBLE));
        }
    }

    if (!(slice && bgoProxy && objProxy && platPathProxy)) {
        model()->removeRow(index, model()->indexOf(model()->root()->row(), 0, model()->root()));
        return;
    }

    if (draw)
        drawEmptySlice(index);
    updateSliceIndex();
}

void PlatzGraphicsView::drawEmptySlice(int index)
{
    delete bgpix;
    bgpix = 0;
    bgpixList.insert(index, QPixmap(mSliceSize.width(),mSliceSize.height()));
    QPainter painter(&bgpixList[index]);
    painter.fillRect(bgpixList.at(index).rect(), QBrush(qRgb(0,0,0)));
    painter.end();
    renderBackbuffer();
    platzScene->setSceneRect(0, 0, (bgpix)?bgpix->width():0, (bgpix)?bgpix->height():0);
}

void PlatzGraphicsView::removeSlice(QPoint pos)
{
    removeSlice((int)mapToScene(pos).x()/bgpixList.at(0).width());  // Change this when we have a manager class
}

void PlatzGraphicsView::removeSlice(int index)
{
    if ((index < 0) || (index >= bgpixList.length()) || !model())
        return;
    model()->setDropState(PlatzDataModel::Valid);
    if (!model()->removeRow(index, model()->indexOf(0, 0, model()->root())))
        return;
    delete bgpix;
    bgpix = 0;
    bgpixList.removeAt(index);
    renderBackbuffer();
    platzScene->setSceneRect(0, 0, (bgpix)?bgpix->width():0, (bgpix)?bgpix->height():0);
    updateSliceIndex();

    // Added this hack since Qt 4.6. Really just want to force an update.
    //update(visibleRegion());
    setBackgroundBrush(backgroundBrush());
}

void PlatzGraphicsView::synchronizeModelView()
{
    // Ensure all canvas sections have a corresponding slice and vice-versa
    while (WorldItem::sliceCount() != sliceCount()) {
        if (WorldItem::sliceCount() < sliceCount())
            insertSlice(WorldItem::sliceCount()-1, 1, false);
        else
            drawEmptySlice(sliceCount());
    }
}

void PlatzGraphicsView::renderBackbuffer()
{
    if (bgpixList.length() == 0)
        return;
    int wid = bgpixList.at(0).width(), hgt = bgpixList.at(0).height();
    delete bgpix;
    bgpix = new QPixmap(bgpixList.length()*wid, hgt);

    QPainter bgPainter(bgpix);

    for (int i = 0; i < bgpixList.length(); i++)
        bgPainter.drawPixmap(i*wid, 0, wid, hgt, bgpixList.at(i));
    bgPainter.end();
}

bool PlatzGraphicsView::loadBackground(QString path)
{
    if (!platzScene || path.isEmpty())
        return false;
    QDir dir(path);
    QFileInfoList fil = dir.entryInfoList(Platz::SUPPORTED_IMAGE_FORMATS, QDir::Files|QDir::Readable, QDir::Name);

    if (fil.length() == 0)
        return false;
    clearBackground();

    QPixmap pixmap, pixSlice(mSliceSize), pixTemp;

    foreach(QFileInfo fi, fil) {
        pixmap = QPixmap(fi.absoluteFilePath());

        if (pixmap.width()%mSliceSize.width())
            continue;
        for (int i = 0; i < pixmap.width(); i+=mSliceSize.width()) {
            pixTemp = pixmap.copy(i, 0, mSliceSize.width(), pixmap.height());
            QPainter painter(&pixSlice);
            painter.fillRect(pixSlice.rect(), QBrush(qRgb(0,0,0)));
            painter.fillRect(pixTemp.rect(), QBrush(pixTemp));
            painter.end();
            bgpixList.append(pixSlice);
        }
    }
    renderBackbuffer();
    platzScene->setSceneRect(0, 0, (bgpix)?bgpix->width():0, (bgpix)?bgpix->height():0);
    enforceCursorRules();
    return true;
}

void PlatzGraphicsView::enforceCursorRules()
{
    if (cursorCurrent && platzScene && platzScene->sceneRect().contains(mapToScene(mousePos)))
        setCursor(*cursorCurrent);
    else
        setCursor(Qt::ArrowCursor);
}

void PlatzGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (zoom) {
        zoomView(event->delta());
        mousePos = event->pos();
        enforceCursorRules();
        event->accept();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void PlatzGraphicsView::zoomView(int delta)
{
    scaleView(pow((double)2, delta / 1024.0));
    update();
}

void PlatzGraphicsView::nextSlice()
{
    if (sliceCount() == 0)
        return;
    setTransformationAnchor(QGraphicsView::NoAnchor);
    translate(((int)mapToScene(0,0).x()%mSliceSize.width())-mSliceSize.width(), 0);
    setTransformationAnchor(AnchorUnderMouse);
    updateSliceIndex();
}

void PlatzGraphicsView::previousSlice()
{
    if (sliceCount() == 0)
        return;
    setTransformationAnchor(QGraphicsView::NoAnchor);
    int offsetX = (int)mapToScene(0,0).x()%mSliceSize.width();
    translate((offsetX)?offsetX:mSliceSize.width(), 0);
    setTransformationAnchor(AnchorUnderMouse);
    updateSliceIndex();
}

void PlatzGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (imode == Platz::IM_INSERT) {
        if (platzScene->sceneRect().contains(mapToScene(mousePos))) {
            if (event->button() == Qt::RightButton)
                insertSlice(event->pos(), 1);
            else
                insertSlice(event->pos(), -1);
        }
    } else if (imode == Platz::IM_REMOVE) {
        if (platzScene->sceneRect().contains(mapToScene(mousePos))) {
            if (event->button()&(Qt::LeftButton|Qt::RightButton))
                removeSlice(event->pos());
        }
    } else if (imode == Platz::IM_REPLICATE) {
        if (platzScene->sceneRect().contains(mapToScene(mousePos))) {
            WorldItem *slice = model()->selectedItem();

            if (slice && (slice->type() == WorldItem::Slice)) {
                int index = ((int)mapToScene(mousePos).x() / mSliceSize.width());

                if (index < sliceCount() && (index <= slice->row() || (event->button()&Qt::LeftButton))) {
                    Slice *replica = static_cast<Slice*>(model()->root()->child(index));

                    if (replica && replica == slice && replica->replicaOf()) {
                        replica->setReplica(0); // Unlink self from replica
                    } else if (replica && (!replica->replicaOf() || (event->button()&Qt::LeftButton)) && slice != replica) {
                        Slice *s = static_cast<Slice*>(slice);

                        // Assign replica if this was not a copy
                        if (event->button()&Qt::RightButton)
                            s->setReplica(replica);
                        // Set these before the insert as they will affect bgo ordering
                        s->setLockedOrdering(replica->lockedOrdering());
                        //s->setBgoOrder(replica->bgoOrder());

                        // Don't allow replicating a replicate or self. Do allow copying of anything except self
                        int row = 0;
                        model()->removeBranch(model()->indexOf(slice->row(), 0, slice));

                        foreach (WorldItem *child, *replica->children())
                            model()->attachBranch(child, slice, row++);
                    } else {
                        slice = 0;
                    }
                    if (slice)
                        model()->setSelectedIndex(model()->indexOf(slice->row(), 0, slice));
                }
            }
        }
    } else if (event->button() == Qt::RightButton) {
        if (dragMode() == QGraphicsView::NoDrag) {
            setDragMode(QGraphicsView::ScrollHandDrag);
            setInteractive(false);
        } else {
            setDragMode(QGraphicsView::NoDrag);
            setInteractive(true);
        }
    } else {
        QGraphicsView::mousePressEvent(event);
        return;
    }
    event->accept();
}

void PlatzGraphicsView::updateSliceIndex()
{
    if (mSliceSize.width() == 0)
        mSliceIndex = 0;
    else
        mSliceIndex = ((int)mapToScene(mousePos).x() / mSliceSize.width());
    int index = qMin(qMax(mSliceIndex, 0), qMax(sliceCount()-1,0)), count = qMax(sliceCount()-1, 0);
    emit sliceIndexChanged(QString::number(index) + " / " + QString::number(count));
}

void PlatzGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    static int x = 0, y = 0;
    static QPointF pt;
    mousePos = event->pos();
    enforceCursorRules();
    QGraphicsView::mouseMoveEvent(event);
    pt = mapToScene(mousePos);

    // Prepare mouseCoordsChanged signal
    bool signal = false;
    int mx = qMin((int)pt.x(), (int)platzScene->width()), my = qMin((int)pt.y(), mSliceSize.height());
    mx = qMax(mx, 0);
    my = qMax(my, 0);

    int xmod = (mTileSize.width()) ? mx%mTileSize.width() : 0;

    if (xmod != x && mSliceSize.width()) {
        x = (mx%mSliceSize.width())-xmod;
        signal = true;
    }
    if ((my&7) != y) {
        y = my - (my&7);
        signal = true;
    }
    if (signal) {
        updateSliceIndex();
        emit mouseCoordsChanged(QString::number(x) + ", " + QString::number(y));
    }


    /*
    if ((mx&7) != x) {
        x = (mx&(mSliceSize.width()-1))-(mx&7);
        signal = true;
    }
    if ((my&7) != y) {
        y = my - (my&7);
        signal = true;
    }
    if (signal) {
        updateSliceIndex();
        emit mouseCoordsChanged(QString::number(x) + ", " + QString::number(y));
    }
    */
/*
    // If this is too jerky, can simply calculate and signal on each event (but it runs cycles relatively high)
    if (!((mx&7) || (my&7))) {
        x = mx&(mSliceSize.width()-1);
        y = my;
    } else if (!(mx&7)) {
        x = mx&mSliceSize.width()-1;
    } else if (!(my&7)) {
        y = my;
    } else {
        return;
    }
    updateSliceIndex();
    emit mouseCoordsChanged(QString::number(x) + ", " + QString::number(y));
*/
}

void PlatzGraphicsView::leaveEvent(QEvent *event)
{
    zoom = false;
    setCursor(Qt::ArrowCursor);
    QGraphicsView::leaveEvent(event);
}

void PlatzGraphicsView::scaleView(qreal scaleFactor)
{
    qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;
    scale(scaleFactor, scaleFactor);
}

void PlatzGraphicsView::deleteSelected()
{
    const QList<PlatzGraphicsItem*>& selection = reinterpret_cast<const QList<PlatzGraphicsItem*>&>(platzScene->selectedPlatzItems());

    while (selection.count()) {
        dataModel->setDropState(PlatzDataModel::Valid);
        dataModel->removeRow(selection[0]->parent()->row(), dataModel->indexOf(selection[0]->parent()->parent()->row(),0,selection[0]->parent()->parent()));
    }
}

void PlatzGraphicsView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Control:
            zoom = true;
            break;
        case Qt::Key_Delete:
            deleteSelected();
            break;
        default:
            QGraphicsView::keyPressEvent(event);
            break;
    }
}

void PlatzGraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Control:
            zoom = false;
            break;
        default:
            QGraphicsView::keyReleaseEvent(event);
            break;
    }
}

void PlatzGraphicsView::itemDrawn(WorldItem *item)
{
    if (!item || (item->type() != WorldItem::Inner && item->type() != WorldItem::Object))
        return;
    QRectF r = item->boundingRect();
    QPixmap pixmap(bgpix->copy((int)r.topLeft().x(), (int)r.topLeft().y(), (int)r.width(), (int)r.height()));

    if (item->type() == WorldItem::Inner) {
        BgInner *bgi = static_cast<BgInner*>(item);

        if (!bgi)
            return;
        if (bgi->flags()&BgInner::BGP)
            emit (mapDrawn(item, pixmap.toImage()));
        else if (bgi->flags()&BgInner::BGA)
            emit (animDrawn(item, pixmap.toImage()));
        else
            emit (tileDrawn(item, pixmap.toImage()));
    } else {
        emit (mapDrawn(item, pixmap.toImage()));
    }
}

void PlatzGraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawBackground(painter, rect);

    if (bgpix)
        painter->drawPixmap(rect, *bgpix, rect);
}

void PlatzGraphicsView::setModel (PlatzDataModel *model)
{
    dataModel = model;
    platzScene->setModel(model);
    connect(dataModel, SIGNAL(rowDragDropped(const QModelIndex&, int, int)), this, SLOT(itemInserted(const QModelIndex&, int, int)));
}

PlatzDataModel* PlatzGraphicsView::model()
{
    return dataModel;
}

void PlatzGraphicsView::setMode(const InteractionMode &mode)
{
    imode = mode;
    cursorCurrent = cursors.at(mode);

    if (dragMode() == QGraphicsView::ScrollHandDrag) {
        setDragMode(QGraphicsView::NoDrag);
        setInteractive(true);
    }
    platzScene->setMode(mode);
    enforceCursorRules();
}

PlatzGraphicsView::~PlatzGraphicsView()
{
    delete platzScene;
    delete bgpix;

    foreach(QCursor *c, cursors)
        delete c;
    cursors.clear();
}
