
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

#include <QtGui>
#include <QHash>
#include <QRectF>
#include <QStringList>
#include <WorldItem.h>
#include <Level.h>
#include <BgOuter.h>
#include <BgInner.h>
#include <BgMutable.h>
#include <BgObject.h>
#include <BgPlatformPath.h>
#include <BgPlatform.h>
#include <ProxyItem.h>
#include "PlatzDataModel.h"

typedef int (PlatzDataModel::*Collect)(WorldItem *w, QStringList &data, bool set);
#define CALL_COLLECT_MFN(object,ptrToMember) ((object).*(ptrToMember))

PlatzDataModel::PlatzDataModel(QObject *parent)
    : QAbstractItemModel(parent), selectedDataItem(0), dropState(Invalid), selectedIndex(QModelIndex()), droppedItem(0)
{
    rootItem = new Level(QList<QVariant>() << "", 0);
    rootItem->appendChild(new Level(QList<QVariant>() << "World", rootItem));
}

PlatzDataModel::~PlatzDataModel()
{
    delete rootItem;
}

void PlatzDataModel::cropWorldData(const QRectF &sliceBounds)
{
    if (!root())
        return;
    QList<WorldItem*> empties;

    foreach (WorldItem *child, *root()->children())
        child->cropBoundingRect(sliceBounds.adjusted(child->offsetX(), 0.0, child->offsetX(), 0.0), empties);
    foreach (WorldItem *empty, empties) {
        setDropState(Valid);
        removeRow(empty->row(), indexOf(empty->parent()->row(), 0, empty->parent()));
    }
}

int PlatzDataModel::replaceStringData(QStringList &data, const WorldItem::StringDataType &type, int from, int to)
{
    return retrieveStringData(data, type, from, to, true);
}

int PlatzDataModel::retrieveStringData(QStringList &data, const WorldItem::StringDataType &type, int from, int to, bool set)
{
    int count = 0;

    if (!root())
        return count;
    if (from == -1)
        from = 0;
    if (to == -1)
        to = root()->childCount()-1;
    to = qMin(to, root()->childCount()-1);

    if (from < 0 || from >= root()->childCount() || from > to)
        return count;
    if (set && data.count() < 2)
        return count;
    Collect mfn;

    switch (type) {
        case WorldItem::TriggerStrings: mfn = &PlatzDataModel::collectTriggers; break;
        case WorldItem::ClrTileStrings: mfn = &PlatzDataModel::collectPlatformClearTiles; break;
        case WorldItem::MutClassIdStrings: mfn = &PlatzDataModel::collectMutableClassIds; break;
        case WorldItem::MutIdStrings: mfn = &PlatzDataModel::collectMutableIds; break;
        default: return count;
    }

    WorldItem *it = root()->child(from);

    while (it->row() <= to) {
        count += CALL_COLLECT_MFN(*this,mfn)(it, data, set);

        if (it->parent())
            it = it->parent()->nextChild(it);
        else
            it = 0;
        if (!it)
            break;
    }
    return count;
}

int PlatzDataModel::collectTriggers(WorldItem *w, QStringList &triggers, bool set)
{
    int count = 0;

    if (!w)
        return count;
    foreach(WorldItem *child, *w->children()) {
        if (child->type() == WorldItem::Outer) {
            BgOuter *bgo = static_cast<BgOuter*>(child);
            QString trigger = bgo->trigger();

            if (set && trigger == triggers.first())
                bgo->setTrigger(triggers.last());
            else if (!set && !trigger.isEmpty() && !triggers.contains(trigger))
                triggers << trigger;
            else
                continue;
            ++count;
        } else if (child->type()&(WorldItem::Slice|WorldItem::OuterProxy)) {
            count += collectTriggers(child, triggers, set);
        }
    }
    return count;
}

int PlatzDataModel::collectPlatformClearTiles(WorldItem *w, QStringList &clrTiles, bool set)
{
    int count = 0;

    if (!w)
        return count;
    foreach(WorldItem *child, *w->children()) {
        if (child->type() == WorldItem::Platform) {
            BgPlatform *plat = static_cast<BgPlatform*>(child);
            QString clrTile = plat->clearTile();

            if (set && clrTile == clrTiles.first())
                plat->setClearTile(clrTiles.last());
            else if (!set && !clrTile.isEmpty() && !clrTiles.contains(clrTile))
                clrTiles << clrTile;
            else
                continue;
            ++count;
        } else if (child->type()&(WorldItem::Slice|WorldItem::PlatformPathProxy|WorldItem::PlatformPath)) {
            count += collectPlatformClearTiles(child, clrTiles, set);
        }
    }
    return count;
}

int PlatzDataModel::collectMutableClassIds(WorldItem *w, QStringList &bgmcIds, bool set)
{
    int count = 0;

    if (!w)
        return count;
    foreach(WorldItem *child, *w->children()) {
        if (child->type() == WorldItem::Inner) {
            BgInner *bgi = static_cast<BgInner*>(child);

            if (bgi->flags()&BgInner::BGMC) {
                QString bgmc = bgi->bgmClass();

                if (set && bgmc == bgmcIds.first())
                    bgi->setBgmClass(bgmcIds.last());
                else if (!set && !bgmc.isEmpty() && !bgmcIds.contains(bgmc))
                    bgmcIds << bgmc;
                else
                    continue;
                ++count;
            }
        } else if (child->type()&(WorldItem::Slice|WorldItem::OuterProxy|WorldItem::Outer)) {
            count += collectMutableClassIds(child, bgmcIds, set);
        }
    }
    return count;
}

int PlatzDataModel::collectMutableIds(WorldItem *w, QStringList &mutStrings, bool set)
{
    int count = 0;

    if (!w)
        return count;
    foreach(WorldItem *child, *w->children()) {
        if (child->type() == WorldItem::Mutable) {
            BgMutable *bgm = static_cast<BgMutable*>(child);
            QString mutString = bgm->mutableString();

            if (set && mutString == mutStrings.first())
                bgm->setMutableString(mutStrings.last());
            else if (!set && !mutString.isEmpty() && !mutStrings.contains(mutString))
                mutStrings << mutString;
            else
                continue;
            ++count;
        } else if (child->type()&(WorldItem::Slice|WorldItem::OuterProxy|WorldItem::Outer)) {
            count += collectMutableIds(child, mutStrings, set);
        }
    }
    return count;
}

QModelIndex PlatzDataModel::validateModel()
{
    WorldItem *invalidItem = root()->validateState();

    if (invalidItem)
        return indexOf(invalidItem->row(), 0, invalidItem);
    return indexOf(-1, -1, 0);
}

// May be cause for a sort children function for all WorldItems based on specified criteria
void PlatzDataModel::sortBgOuters(Slice *slice)
{
    if (!slice || slice->lockedOrdering())
        return;
    int bgoOrder = slice->bgoOrder();
    ProxyItem *proxy = slice->outerProxy();

    if ((bgoOrder != -1 && bgoOrder != 1) || !proxy || !proxy->childCount())
        return;
    QList<WorldItem*> children = *proxy->children();

    foreach (WorldItem *child, children) {
        attachBranch(child, proxy, child->row());
        setDropState(Valid);
        removeRow(child->row(), indexOf(proxy->row(), 0, proxy));
    }
    emit bgoSortComplete(slice);
}

void PlatzDataModel::updateCurrentIndex(const QModelIndex &parent, int start, int)
{
    if (droppedItem)
        return;
    WorldItem *w = instanceOf(parent);

    if (!w)
        return;
    // To match treeview, we go down within the current branch level until at the end - then go up
    int currCount = w->childCount();

    if (currCount > start)
        emit currentIndexChanged(indexOf(start, 0, w->child(start)));
    else if (currCount)
        emit currentIndexChanged(indexOf(start-1, 0, w->child(start-1)));
    else
        emit currentIndexChanged(indexOf(w->row(), 0, w));
}

void PlatzDataModel::selectDroppedItem()
{
    if (droppedItem)
        setSelectedIndex(indexOf(droppedItem->row(), 0, droppedItem));
    droppedItem = 0;
}

void PlatzDataModel::setSelectedIndex(const QModelIndex &index)
{
    if (index.isValid()) {
        selectedIndex = index;
        emit selectedIndexChanged(selectedIndex);

        selectedDataItem = instanceOf(index);

        if (selectedDataItem->parent())
            emit selectedIndexParentChanged(indexOf(selectedDataItem->parent()->row(), 0, selectedDataItem->parent()));
    } else {
        selectedDataItem = 0;
    }
}

WorldItem* PlatzDataModel::root()
{
    if (rootItem)
        return rootItem->child(0);
    else
        return 0;
}

void PlatzDataModel::clear()
{
    WorldItem *r = root();

    if (!r)
        return;

    while(r->childCount()) {
        setDropState(Valid);
        removeRow(0, createIndex(r->child(0)->row(), 0, r));
    }
}

QModelIndex PlatzDataModel::indexOf(int row, int column, WorldItem* item)
{
    return createIndex(row,column,item);
}

WorldItem* PlatzDataModel::instanceOf(const QModelIndex &index)
{
    if (!index.isValid())
        return 0;
    else
        return static_cast<WorldItem*>(index.internalPointer());
}

int PlatzDataModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<WorldItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant PlatzDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if ((role != Qt::DisplayRole) && (role != Qt::EditRole) && (role != Qt::ToolTipRole) && (role != Qt::DecorationRole))
        return QVariant();

    WorldItem *item = static_cast<WorldItem*>(index.internalPointer());

    if (role == Qt::DecorationRole)
        return item->dataDecoration(0);
    else if (role == Qt::ToolTipRole)
        return item->tooltipData(index.column());
    else
        return item->data(index.column());
}

bool PlatzDataModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        WorldItem *item = static_cast<WorldItem*>(index.internalPointer());
        item->setData(value);
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

void PlatzDataModel::packDataBranch(const QModelIndex &index, int role, PlatzDataStream &stream) const
{
    if (!index.isValid())
        return;
    if ((role != Qt::DisplayRole) && (role != Qt::EditRole) && (role != Qt::ToolTipRole))
        return;

    WorldItem *item = static_cast<WorldItem*>(index.internalPointer());
    stream << item;
/*
    if (item->childCount()) {
        const QList<WorldItem*> *children = item->children();

        foreach(WorldItem* wi, *children)
            packDataBranch(createIndex(wi->row(), 0, wi), role, stream);
    }
*/
}

Qt::ItemFlags PlatzDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

QVariant PlatzDataModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex PlatzDataModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    WorldItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<WorldItem*>(parent.internalPointer());

    WorldItem *childItem = parentItem->child(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex PlatzDataModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    WorldItem *childItem = static_cast<WorldItem*>(index.internalPointer());
    WorldItem *parentItem = childItem->parent();

    Q_ASSERT_X(parentItem, "PlatzDataModel::parent()", "NULL parent.");

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int PlatzDataModel::rowCount(const QModelIndex &parent) const
{
    WorldItem *parentItem;

    //if (parent.column() > 0)
    //    return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<WorldItem*>(parent.internalPointer());

    return parentItem->childCount();
}

bool PlatzDataModel::setHeaderData(int, Qt::Orientation, const QVariant&, int) {
    return false;
}

Qt::DropActions PlatzDataModel::supportedDropActions() const
{
     return Qt::MoveAction;
}

bool PlatzDataModel::insertRow (int row, const QModelIndex &parent)
{
    return insertRows(row, 1, parent);
}

bool PlatzDataModel::insertRow(int row, WorldItem *child,  const QModelIndex &parent)
{
    if (!parent.isValid())
        return false;
    beginInsertRows(parent, row, row);
    WorldItem *parentItem = static_cast<WorldItem*>(parent.internalPointer());
    parentItem->insertChild(row, child);
    endInsertRows();
    emit rowDragDropped(parent, row, row);
    return true;
}

bool PlatzDataModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (!parent.isValid())
        return false;
    beginInsertRows(parent, row, row+count-1);
    QList<QVariant> data;
    data << "";
    WorldItem *parentItem = static_cast<WorldItem*>(parent.internalPointer());

    for (int i = row; i < (row+count); i++) {
        WorldItem *childItem = parentItem->createItem(data);
        parentItem->insertChild(i, childItem);
    }
    endInsertRows();
    return true;
}

bool PlatzDataModel::removeRow(int row, const QModelIndex &parent)
{
    return removeRows(row, 1, parent);
}

bool PlatzDataModel::removeRows (int row, int count, const QModelIndex &parent)
{
    if (dropState != Valid)
        return false;
    dropState = Invalid;

    if (!parent.isValid())
        return false;
    WorldItem *parentItem = instanceOf(parent);
    row = qMin(parentItem->childCount(), row);
    count = qMin(parentItem->childCount()-row, count);

    if (parentItem->type() == WorldItem::Level) {
        for (int i = row; i < parentItem->childCount(); i++) {
            WorldItem *child = parentItem->child(i);

            if (child) {
                if (child->childCount())
                    updateGeometries(child);
                if (child->graphicalRepresentation())
                    child->graphicalRepresentation()->platzPrepareGeometryChange();
            }
        }
    }

    for (int i = row; i < (row+count); i++)
        if (!removeBranch(createIndex(i, 0, parentItem->child(i))))
            return false;
    if (count) {
        beginRemoveRows(parent,row,row+count-1);
        bool retval = parentItem->removeChildRows(row, count);
        endRemoveRows();

        if (!retval)
            return false;
    }
    setSelectedIndex(indexOf(-1, -1, 0));   // Set to invalid
    if (droppedItem)
        selectDroppedItem();
    updateCurrentIndex(parent, row, row+count-1);
    return true;
}

void PlatzDataModel::updateGeometries(WorldItem *branchRoot)
{
    foreach (WorldItem *child, *branchRoot->children()) {
        if (child->childCount())
            updateGeometries(child);
        if (child->graphicalRepresentation())
            child->graphicalRepresentation()->platzPrepareGeometryChange();
    }
}

bool PlatzDataModel::removeBranch(const QModelIndex &parent)
{
    if (!parent.isValid())
        return false;

    WorldItem *parentItem = static_cast<WorldItem*>(parent.internalPointer());
    QList<WorldItem*>* children =  const_cast<QList<WorldItem*>*>(parentItem->children());

    if (parentItem->childCount()) {
        for (int i = 0; i < children->count(); i++)
            if (!removeBranch(createIndex(i,0,parentItem->child(i))))
                return false;
        beginRemoveRows(parent,0,parentItem->childCount()-1);
        bool retval = parentItem->removeChildRows(0, parentItem->childCount());
        endRemoveRows();
        return retval;
    } else {
        return true;    // No children ends recursion
    }
}

void PlatzDataModel::setDropState(PlatzDataModel::DropState state)
{
    dropState = state;
}

QStringList PlatzDataModel::mimeTypes () const
{
    return QStringList("application/platztree");
}

// I don't like how this and dropMimeData work. If dropMimeData and removeRows are called
// in a different order by the view, then we have stray pointers.
QMimeData* PlatzDataModel::mimeData (const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;
    
    if (indexes.count() > 1)
        return mimeData;

    PlatzDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes)
        if (index.isValid())
            packDataBranch(index, Qt::DisplayRole, stream);

    mimeData->setData("application/platztree", encodedData);
    return mimeData;
}

WorldItem* PlatzDataModel::attachBranch(WorldItem *branchRoot, WorldItem *trunk, int row)
{
    if (!branchRoot || !trunk)
        return 0;
    if (trunk->type() == WorldItem::Level) {
        for (int i = row; i < trunk->childCount(); i++) {
            WorldItem *slice = trunk->child(i);

            if (slice) {
                if (slice->childCount())
                    updateGeometries(slice);
                if (slice->graphicalRepresentation())
                    slice->graphicalRepresentation()->platzPrepareGeometryChange();
            }
        }
    }

    WorldItem *w = branchRoot->createItem(QList<QVariant>() << WorldItem::baseData(branchRoot->type()), trunk);

    if (!w)
        return 0;
    w->setRelativeBoundingRect(branchRoot->relativeBoundingRect());
    // BgOuter left-to-right order must be preserved
    if (w->type() == WorldItem::Outer) {
        WorldItem *proxy = w->parent();

        if (proxy && proxy->parent()) {
            Slice *slice = static_cast<Slice*>(proxy->parent());

            if (slice && !slice->lockedOrdering()) {
                row = 0;

                foreach (WorldItem *child, *w->parent()->children()) {
                    if (child) {
                        if (w->relativeBoundingRect().left() <= child->relativeBoundingRect().left()) {
                        //if ((slice->bgoOrder() == 1 && w->relativeBoundingRect().left() <= child->relativeBoundingRect().left()) ||
                        //        (slice->bgoOrder() == -1 && w->relativeBoundingRect().right() >= child->relativeBoundingRect().right())) {
                            row = child->row();
                            break;
                        }
                    }
                    ++row;
                }
            }
        }
    } else if (w->type() == WorldItem::Mutable) {
        BgInner *bgi = static_cast<BgInner*>(trunk->child(row-1));
        BgInner *bgm = static_cast<BgInner*>(w);

        if (!(bgi && bgm))
            return 0;
        bgi->setMutator(bgm);
        bgm->setMutator(bgi);
        bgm->setRelativeBoundingRect(branchRoot->relativeBoundingRect());
    }
    if (!insertRow(row, w, indexOf(trunk->row(), 0, trunk)))
        return 0;
    foreach(WorldItem *twig, *branchRoot->children())
        if (!attachBranch(twig, w, w->childCount()))
            return 0;
    return w;
}

bool PlatzDataModel::dropMimeData (const QMimeData *data, Qt::DropAction action, int row, int, const QModelIndex &parent)
{
    if (!data->hasFormat("application/platztree"))
        return false;
    if (action == Qt::IgnoreAction)
        return true;
    if (!parent.isValid())
        return false;

    QByteArray encodedData = data->data("application/platztree");
    PlatzDataStream stream(&encodedData, QIODevice::ReadOnly);
    WorldItem *w = 0, *parentItem = static_cast<WorldItem*>(parent.internalPointer());
    row = qMax(row,0);

    if (!stream.atEnd()) {  // Only process one item (a single branch)
        stream >> &w;

        if (!w || !parentItem->validChild(w->type()))
            return false;
        droppedItem = attachBranch(w, parentItem, row);

        if (!droppedItem)
            return false;
    }
    setDropState(Valid);
    return true;
}

bool PlatzDataModel::setItemData (const QModelIndex&, const QMap<int, QVariant>&)
{
    return true;
}
