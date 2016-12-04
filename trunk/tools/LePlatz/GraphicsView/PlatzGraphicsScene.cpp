
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

#include "PlatzGraphicsScene.h"
#include "PlatzGraphicsItem.h"
#include <QPen>
#include <Slice.h>
#include <BgOuter.h>
#include <BgMutable.h>
#include <BgObject.h>

QList<QPen> PlatzGraphicsScene::pens;
QList<QBrush> PlatzGraphicsScene::brushes;

void PlatzGraphicsScene::initSceneDrawTools()
{
    QPen pen;
    QBrush brush(Qt::SolidPattern);

    pens.clear();
    brushes.clear();
    pen.setWidth(2);
    pen.setCosmetic(true);
    pen.setStyle(Qt::DashLine);
    pen.setColor(QColor(0,255,0));
    brush.setColor(QColor(0,255,0,50));     // Green
    pens.append(pen);
    brushes.append(brush);
    pen.setColor(QColor(0,0,255));
    brush.setColor(QColor(0,0,255,50));   // Blue
    pens.append(pen);
    brushes.append(brush);
    pen.setColor(QColor(24,244,241));
    brush.setColor(QColor(24,244,241,50));   // Aqua
    pens.append(pen);
    brushes.append(brush);
    pen.setColor(QColor(240,100,240));
    brush.setColor(QColor(240,100,240,50)); // Purple
    pens.append(pen);
    brushes.append(brush);
    pen.setColor(QColor(255,255,0));
    brush.setColor(QColor(255,255,0,50));   // Yellow
    pens.append(pen);
    brushes.append(brush);
    pen.setColor(QColor(255,130,40));
    brush.setColor(QColor(255,150,10,50));  // Orange
    pens.append(pen);
    brushes.append(brush);
    pen.setColor(QColor(255,255,255));
    brush.setColor(QColor(255,0,0,75)); // Red
    pens.append(pen);
    brushes.append(brush);
    /*
    pen.setColor(QColor(255,255,255,0));
    brush.setColor(QColor(255,255,255,0));  // Clear
    pens.append(pen);
    brushes.append(brush);
    */
}

void PlatzGraphicsScene::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{
    WorldItem *w = dataModel->instanceOf(parent);

    if (!w)
        return;

    for (int i = start; i <= end; i++) {
        WorldItem *child = w->child(i);

        if (child && child->graphicalRepresentation())
            removeSelection(child->graphicalRepresentation());
    }
}

PlatzGraphicsScene::PlatzGraphicsScene(QObject *parent)
    : QGraphicsScene(parent), bgOutline(0.0,0.0,0.0,0.0), state(NoDraw), item(0), sel(0), sceneMode(IM_SELECT),
        row(0), parent(QModelIndex()), selectionHidden(false), useCustomPayload(false), mutParent(0),
        snapToResolutionX(8), snapToResolutionY(8)
{
}

void PlatzGraphicsScene::setSnapToResolutionX(int resolution)
{
    if (resolution > 0)
        snapToResolutionX = resolution;
}

void PlatzGraphicsScene::setSnapToResolutionY(int resolution)
{
    if (resolution > 0)
        snapToResolutionY = resolution;
}

void PlatzGraphicsScene::clearOuterJoin()
{
    if (join.count()) {
        foreach (PlatzGraphicsItem* pitem, join)
            pitem->setVisible(true);
    }
    join.clear();
}

void PlatzGraphicsScene::hideOuterJoin(const InteractionMode &mode)
{
    WorldItem::WorldItemType type;

    switch (mode) {
        case IM_SELECT:
            clearOuterJoin();
            return;
        case IM_OUTER_BG:
            type = WorldItem::Outer;
            break;
        case IM_INNER_BG:
            type = WorldItem::Inner;
            break;
        case IM_OBJECT_BG:
            type = WorldItem::Object;
            break;
        case IM_PLATFORM_PATH:
            type = WorldItem::PlatformPath;
            break;
        case IM_PLATFORM:
            type = WorldItem::Platform;
            break;
        case IM_MUTABLE_BG:
            type = WorldItem::Mutable;
            break;
        default:
            return;
    }
    clearOuterJoin();
    QList<QGraphicsItem*> temp = items();
    QList<PlatzGraphicsItem*> *sel = reinterpret_cast<QList<PlatzGraphicsItem*>*>(&temp);

    foreach (PlatzGraphicsItem *pitem, *sel) {
        if (pitem->parent()->type() != type) {
            pitem->setVisible(false);
            join.append(pitem);
        }
    }
}

void PlatzGraphicsScene::setSelection(const QModelIndex &index)
{
    if (state != NoDraw)
        return;
    cancelSelection();

    if (!index.isValid())
        return;

    WorldItem *w = model()->instanceOf(index);

    if (w) {
        PlatzGraphicsItem *sel = w->graphicalRepresentation();

        if (sel) {
            // Don't add containers to the scene
            if (sel->mode() != Platz::INVISIBLE) {
                sel->setMode(Platz::SELECTED);
                selection.append(sel);
            }
        }
        //emit currentItemChanged(w);
        emitIndexChanges(w);
    }
    emit interactionModeChanged(sceneMode);
}

void PlatzGraphicsScene::selectAll()
{
    cancelSelection();
    appendSelection(items());

    QList<PlatzGraphicsItem*> *pitems = reinterpret_cast<QList<PlatzGraphicsItem*>*>(&selection);

    foreach (PlatzGraphicsItem *sel, *pitems)
        sel->setMode(Platz::SELECTED);
    emit (interactionModeChanged(sceneMode));
}

void PlatzGraphicsScene::appendSelection(QGraphicsItem *selItem)
{
    if (selItem)
        selection.append(selItem);
}

void PlatzGraphicsScene::appendSelection(const QList<QGraphicsItem*> &selItem)
{
    selection.append(selItem);
}

bool PlatzGraphicsScene::removeSelection(QGraphicsItem *selItem)
{
    return selection.removeOne(selItem);
}

const QList<QGraphicsItem*>& PlatzGraphicsScene::selectedPlatzItems()
{
    return selection;
}

void PlatzGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {

    if (state != NoDraw)
        return;

    QPointF p = mouseEvent->scenePos();
    clampToScene(p);

    if ((sceneMode != IM_MUTABLE_BG) && (sceneMode == IM_SELECT || !selectionHidden))
        cancelSelection();

    Q_ASSERT_X(dataModel, "PlatzGraphicsScene::mousePressEvent", "NULL dataModel");

    // Mutable parent has one turn lifespan after a BGM BgInner is created (for convenience)
    if (sceneMode != IM_MUTABLE_BG)
        setMutableParent(0);
    item = 0;

    switch (sceneMode) {
        case IM_PLATFORM:
        {
            clampToGrid(p);
            bgOutline.setTopLeft(p);
            bgOutline.setBottomRight(p);
            QList<QGraphicsItem*> selectedItems(items(p));
            PlatzGraphicsItem *platPathItem;
            WorldItem *platPath;

            if (!selectedItems.count())
                return;
            foreach(QGraphicsItem *s, selectedItems) {
                platPathItem = static_cast<PlatzGraphicsItem*>(s);
                platPath = platPathItem->parent();
                if (platPath->type() == WorldItem::PlatformPath) {
                    //item->setFlags(QGraphicsItem::ItemIsSelectable);
                    // Allow only one platform per platform path
                    if (platPath->childCount()) {
                        dataModel->setDropState(PlatzDataModel::Valid);
                        dataModel->removeRow(0, dataModel->indexOf(platPath->row(), 0, platPath));
                    }
                    BgPlatform *plat = new BgPlatform(QList<QVariant>() << "Platform", platPath);
                    plat->setFlags(platformStyle);
                    plat->setClearTile(platformClrTile);
                    plat->setVelocity(platformVel);
                    plat->setBoundingRect(bgOutline);
                    item = new PlatzGraphicsItem(plat);
                    plat->setGraphicalRepresentation(item);
                    row = platPath->childCount();
                    parent = dataModel->indexOf(platPath->row(), 0, platPath);
                    state = BeginDraw;
                    dataModel->insertRow(row, plat, parent);
                    emit (selectionChanged(dataModel->indexOf(row, 0, plat)));
                    break;
                }
            }
            break;
        }
        case IM_PLATFORM_PATH:
        {
            clampToGrid(p);
            bgOutline.setTopLeft(p);
            bgOutline.setBottomRight(p);
            Slice *slice = static_cast<Slice*>(dataModel->root()->child((int)p.x()/mSliceSize.width()));
            WorldItem *proxy = slice->platformProxy();
            //item->setFlags(QGraphicsItem::ItemIsSelectable);
            BgPlatformPath *platPath = new BgPlatformPath(QList<QVariant>() << "PlatformPath", proxy);
            platPath->setAxis(platformAxis);
            platPath->setBoundingRect(bgOutline);
            item = new PlatzGraphicsItem(platPath);
            platPath->setGraphicalRepresentation(item);
            row = proxy->childCount();
            parent = dataModel->indexOf(proxy->row(), 0, proxy);
            state = BeginDraw;
            dataModel->insertRow(row, platPath, parent);
            emit (selectionChanged(dataModel->indexOf(row, 0, platPath)));
            break;
        }
        case IM_OBJECT_BG:
        {
            clampToGrid(p);
            bgOutline.setTopLeft(p);
            bgOutline.setBottomRight(p);
            Slice *slice = static_cast<Slice*>(dataModel->root()->child((int)p.x()/mSliceSize.width()));
            WorldItem *proxy = slice->objectProxy();
            //item->setFlags(QGraphicsItem::ItemIsSelectable);
            BgObject *obj = new BgObject(QList<QVariant>() << "BgObject", proxy);
            obj->setBoundingRect(bgOutline);
            item = new PlatzGraphicsItem(obj);
            obj->setGraphicalRepresentation(item);
            row = proxy->childCount();
            state = BeginDraw;
            parent = dataModel->indexOf(proxy->row(), 0, proxy);
            dataModel->insertRow(row, obj, parent);
            emit (selectionChanged(dataModel->indexOf(row, 0, obj)));
            break;
        }
        case IM_INNER_BG:
        {
            clampToGrid(p);
            bgOutline.setTopLeft(p);
            bgOutline.setBottomRight(p);
            QList<QGraphicsItem*> selectedItems(items(p));
            PlatzGraphicsItem *bgoItem;
            WorldItem *bgo;
            
            if (!selectedItems.count())
                return;
            foreach(QGraphicsItem *s, selectedItems) {
                bgoItem = static_cast<PlatzGraphicsItem*>(s);
                bgo = bgoItem->parent();

                if (bgo->type() == WorldItem::Outer && !(static_cast<BgOuter*>(bgo)->flags()&BgOuter::BGT)) {
                    //item->setFlags(QGraphicsItem::ItemIsSelectable);
                    BgInner *bgi = new BgInner(QList<QVariant>() << "BgInner", bgo);
                    bgi->setFlags(bgiFlags);
                    bgi->setTile(bgiTile);
                    bgi->setBoundingRect(bgOutline);
                    item = new PlatzGraphicsItem(bgi);
                    bgi->setGraphicalRepresentation(item);
                    row = bgo->childCount();
                    parent = dataModel->indexOf(bgo->row(), 0, bgo);
                    state = BeginDraw;
                    dataModel->insertRow(row, bgi, parent);

                    if (bgi->flags()&BgInner::BGM)
                        setMutableParent(bgi);
                    else if (bgi->flags()&BgInner::BGMC)
                        bgi->setBgmClass(bgmc);
                    emit (selectionChanged(dataModel->indexOf(row, 0, bgi)));
                    break;
                }
            }
            break;
        }
        case IM_MUTABLE_BG:
        {
            BgInner *bgi = 0;

            if (mutableParent()) {
                bgi = mutableParent();
                setMutableParent(0);
            } else if (selectedPlatzItems().count()) {
                const PlatzGraphicsItem *bgiItem = static_cast<const PlatzGraphicsItem*>(selectedPlatzItems().at(0));

                if (!bgiItem->parent())
                    break;
                bgi = static_cast<BgInner*>(bgiItem->parent());
            } else {
                break;
            }

            clampToGrid(p);
            bgOutline.setTopLeft(p);
            bgOutline.setBottomRight(p);

            if (!bgi->parent() || bgi->type() != WorldItem::Inner || !(bgi->flags()&BgInner::BGM))
                return;
            BgOuter *bgo = static_cast<BgOuter*>(bgi->parent());

            if (!bgo->children()->contains(bgi))
                return;
            Slice *slice = static_cast<Slice*>(dataModel->root()->child((int)p.x()/mSliceSize.width()));

            if (!slice->boundingRect().contains(bgi->boundingRect()))
                return;
            if (bgi->mutator()) {
                dataModel->setDropState(PlatzDataModel::Valid);
                dataModel->removeRow(bgi->mutator()->row(), dataModel->indexOf(bgo->row(), 0, bgo));
                bgi->setMutator(0);
            }
            BgMutable *bgm = new BgMutable(QList<QVariant>() << "BgMutable", bgi, bgo);
            bgm->setFlags(0);
            bgm->setTile(bgiTile);
            bgm->setMutableString(mutString);
            bgm->setBoundingRect(bgOutline);

            if (useCustomPayload)
                bgm->setCustomPayload(customPayload);
            else
                bgm->setPayload(bgm->relativeBoundingRect().toRect());
            item = new PlatzGraphicsItem(bgm);
            bgm->setGraphicalRepresentation(item);
            bgi->setMutator(bgm);
            row = bgo->children()->indexOf(bgi)+1;
            parent = dataModel->indexOf(bgo->row(), 0, bgo);
            state = BeginDraw;
            dataModel->insertRow(row, bgm, parent);
            emit (selectionChanged(dataModel->indexOf(row, 0, bgm)));
            break;
        }
        case IM_OUTER_BG:
        {
            clampToGrid(p);
            bgOutline.setTopLeft(p);
            bgOutline.setBottomRight(p);
            Slice *slice = static_cast<Slice*>(dataModel->root()->child((int)p.x()/mSliceSize.width()));
            WorldItem *proxy = slice->outerProxy();
            BgOuter *bgo = new BgOuter(QList<QVariant>() << "BgOuter", proxy);
            bgo->setFlags(bgoFlags);

            if (bgoFlags&BgOuter::BGT) {
                bgo->setTrigger(trigId);
                bgo->setTriggerOrientation(trigOrientation);
            }
            bgo->setBoundingRect(bgOutline);
            item = new PlatzGraphicsItem(bgo);
            bgo->setGraphicalRepresentation(item);
            row = proxy->childCount();
            parent = dataModel->indexOf(proxy->row(), 0, proxy);
            state = BeginDraw;
            dataModel->insertRow(row, bgo, parent);
            emit (selectionChanged(dataModel->indexOf(row, 0, bgo)));
            break;
        }
        case IM_SELECT:
        {
            bgOutline.setTopLeft(p);
            bgOutline.setBottomRight(p);
            sel = new PlatzGraphicsItem(0, Platz::SELECTED);
            sel->setZValue(100);
            // mSliceSize mutator doesn't allow for empty slices, so no DBZ check needed
            sel->setOffsetX((int)p.x()-((int)p.x()%mSliceSize.width()));
            sel->setBoundingRect(bgOutline);
            addItem(sel);
            state = BeginDraw;
            break;
        }
        default:
            break;
    }
    mouseEvent->accept();
}

void PlatzGraphicsScene::mouseMoveEvent (QGraphicsSceneMouseEvent *mouseEvent) {
    QRectF rectNorm;

    if (state != BeginDraw)
        return;
    QPointF p = mouseEvent->scenePos();
    clampToScene(p);

    switch (sceneMode) {
        case IM_SELECT:
            bgOutline.setBottomRight(p);
            rectNorm = (bgOutline.isEmpty())?bgOutline.normalized():bgOutline;
            if (sel)
                sel->setBoundingRect(rectNorm);
            break;
        default:
            clampToGrid(p);
            bgOutline.setBottomRight(p);
            rectNorm = (bgOutline.isEmpty())?bgOutline.normalized():bgOutline;
            if (item)
                item->setBoundingRect(rectNorm);
            break;
    }
    update();
    mouseEvent->accept();
}

void PlatzGraphicsScene::mouseReleaseEvent (QGraphicsSceneMouseEvent *mouseEvent) {
    if (state == NoDraw)
        return;
    switch (sceneMode) {
        case IM_SELECT:
        {
            removeItem(sel);
            delete (sel);
            sel = NULL;

            // Prepare for following tests
            if (!bgOutline.isValid())
                bgOutline = bgOutline.normalized();
            // Allow mouse click/releases outside of the scene to clear the selection
            if (bgOutline.width() < 1 && bgOutline.height() < 1 && !sceneRect().contains(mouseEvent->scenePos()))
                break;
            // Ensure a non-zero area so that point selection doesn't fail
            if (bgOutline.height() == 0)
                bgOutline.setBottom(bgOutline.bottom()+1);
            if (bgOutline.width() == 0)
                bgOutline.setRight(bgOutline.right()+1);
            selection.clear();
            selection.append(items(bgOutline, Qt::IntersectsItemBoundingRect));
            makeSelection();

            WorldItem *p = 0;

            if (selection.count()) {
                const PlatzGraphicsItem *pitem = static_cast<PlatzGraphicsItem*>(selection.at(0));

                if (pitem)
                    p = pitem->parent();
            }
            //emit currentItemChanged(p);
            emitIndexChanges(p);
            break;
        }
        default:
        {
            if (!item)
                break;
            if (item->boundingRect().isEmpty()) {
                dataModel->setDropState(PlatzDataModel::Valid); // Don't like this hack
                dataModel->removeRow(row, parent);
            } else {
                WorldItem *p = item->parent();

                // TODO: This functionality should be encapsulated by the model
                if (p) {
                    // Ensure BgOuters are ordered left-to-right for collision detection simplification
                    if (p->type() == WorldItem::Outer) {
                        BgOuter *bgo = static_cast<BgOuter*>(p);
                        ProxyItem *proxy = static_cast<ProxyItem*>(bgo->parent());

                        if (!proxy)
                            break;
                        Slice *slice = static_cast<Slice*>(proxy->parent());

                        if (!slice || slice->lockedOrdering())
                            break;
                        const QList<BgOuter*> *bgoChildren = reinterpret_cast<const QList<BgOuter*>*>(proxy->children());
                        const BgOuter *bgoIndex = bgo, *bgoIt = 0;

                        for (int i = proxy->childCount()-1; i >= 0; i--) {
                            bgoIt = bgoChildren->at(i);

                            if (bgoIt) {
                                if ((slice->bgoOrder() == 1 && bgo->relativeBoundingRect().left() < bgoIt->relativeBoundingRect().left()) ||
                                        (slice->bgoOrder() == -1 && bgo->relativeBoundingRect().right() > bgoIt->relativeBoundingRect().right()))
                                    bgoIndex = bgoIt;
                            }
                        }
                        if (bgoIndex && bgoIndex != bgo && proxy->child(bgoIndex->row())) {
                            BgOuter *bgoCopy = static_cast<BgOuter*>(bgo->createItem(QList<QVariant>() << WorldItem::baseData(bgo->type())));
                            dataModel->setDropState(PlatzDataModel::Valid);
                            dataModel->removeRow(row, parent);
                            row = bgoIndex->row();
                            dataModel->insertRow(row, bgoCopy, parent);
                            p = bgoCopy;
                            emit (selectionChanged(dataModel->indexOf(row, 0, p)));
                        }
                    } else if (!useCustomPayload && p->type() == WorldItem::Mutable) {
                        BgMutable *bgm = static_cast<BgMutable*>(p);

                        if (!bgm->isCustomPayload())
                            bgm->setPayload(bgm->relativeBoundingRect().toRect());
                    }
                    emit itemDrawn(p);
                }
                emitIndexChanges(p);
            }
            item = 0;
            break;
        }
    }
    state = NoDraw;
    update();
    mouseEvent->accept();
}

void PlatzGraphicsScene::emitIndexChanges(const WorldItem *witem)
{
    if (!witem) {
        emit currentTileIndexChanged(Platz::CLEAR);
    } else if (witem->type() == WorldItem::Inner) {
        const BgInner *bgi = static_cast<const BgInner*>(witem);

        if (bgi) {
            if (bgi->flags()&BgInner::BGP)
                emit currentMapIndexChanged(bgi->tile());
            else if (bgi->flags()&BgInner::BGA)
                emit currentAnimIndexChanged(bgi->tile());
            else
                emit currentTileIndexChanged(bgi->tile());
        }
    } else if (witem->type() == WorldItem::Object) {
        const BgObject *obj = static_cast<const BgObject*>(witem);

        if (obj)
            emit currentMapIndexChanged(obj->map());
    } else {
        emit currentTileIndexChanged(Platz::CLEAR);
    }
    emit currentItemDataChanged(witem);
}

Platz::InteractionMode PlatzGraphicsScene::interactionMode()
{
    return sceneMode;
}

const InteractionMode& PlatzGraphicsScene::mode()
{
    return sceneMode;
}

void PlatzGraphicsScene::setMode(const InteractionMode &im) {
    switch (im) {
        case IM_SELECT:
            showSelection();
            break;
        case IM_REPLICATE:
        case IM_MUTABLE_BG:
            break;
        default:
            if (!selectionHidden)
                cancelSelection();
            break;
    }

    sceneMode = im;
    emit interactionModeChanged(sceneMode);
    update();
}

void PlatzGraphicsScene::makeSelection(bool emitSignal) {
    PlatzGraphicsItem *it = 0;

    selectionHidden = false;

    for (int i = 0; i < selection.count(); i++) {
        it = static_cast<PlatzGraphicsItem*>(selection[i]);

        if (it) {
            if (i == 0 && emitSignal)
                emit(selectionChanged(dataModel->indexOf(it->parent()->row(), 0, it->parent())));
            it->setVisible(true);
            it->setMode(Platz::SELECTED);
        }
    }
}

void PlatzGraphicsScene::showSelection(void) {
    makeSelection(false);
}

bool PlatzGraphicsScene::isSelectionVisible()
{
    return !selectionHidden;
}

void PlatzGraphicsScene::hideSelection() {
    selectionHidden = true;
    
    for (int i = 0; i < selection.count(); i++)
        static_cast<PlatzGraphicsItem*>(selection[i])->setVisible(false);
}

void PlatzGraphicsScene::cancelSelection(void) {
    PlatzGraphicsItem *it = 0;

    for (int i = 0; i < selection.count(); i++) {
        it = static_cast<PlatzGraphicsItem*>(selection[i]);

        if (it) {
            if (!it->isVisible())
                it->setVisible(true);
            it->setMode(Platz::NORMAL);
        }
    }
    selection.clear();
}

void PlatzGraphicsScene::clampToScene(QPointF &p) {
    if (p.x() < this->sceneRect().left())
        p.setX(this->sceneRect().left());
    else if (p.x() > this->sceneRect().right())
        p.setX(this->sceneRect().right());

    if (p.y() < this->sceneRect().top())
        p.setY(this->sceneRect().top());
    else if (p.y() > this->sceneRect().bottom())
        p.setY(this->sceneRect().bottom());
}

// Clamp to 8x8-pixel grid
void PlatzGraphicsScene::clampToGrid(QPointF &p) {
    snapToResolutionX = qMax(snapToResolutionX, 1);
    snapToResolutionY = qMax(snapToResolutionY, 1);

    int x = (int)p.x(), y = (int)p.y();
    int xMod = x%snapToResolutionX, yMod = y%snapToResolutionY;

    if (xMod) {
        if (xMod < (snapToResolutionX/2))
            x -= xMod;
        else
            x += snapToResolutionX-xMod;
    }

    if (yMod) {
        if (yMod < (snapToResolutionY/2))
            y -= yMod;
        else
            y += snapToResolutionY-yMod;
    }
    p.setX(x);
    p.setY(y);
}

void PlatzGraphicsScene::setModel (PlatzDataModel *model)
{
    dataModel = model;
    connect(dataModel, SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)), this, SLOT(rowsAboutToBeRemoved(QModelIndex, int, int)));
}

PlatzDataModel* PlatzGraphicsScene::model()
{
    return dataModel;
}

PlatzGraphicsScene::~PlatzGraphicsScene()
{
    Q_ASSERT_X(item == 0, "~PlatzDataModel()", "PlatzGraphicsItem pointer wasn't reset after draw.");
    delete sel;
}

