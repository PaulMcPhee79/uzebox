
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

#ifndef PLATZDATAMODEL_H
#define PLATZDATAMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <WorldItem.h>
#include <Slice.h>
#include <PlatzDataStream.h>

class PlatzDataModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum DropState {
        Invalid,
        Valid
    };

    PlatzDataModel(QObject *parent = 0);
    ~PlatzDataModel();

    // Probably a bad idea to double-up as set/get, but it's only used from FindReplace anyway, so no big deal
    int replaceStringData(QStringList &data, const WorldItem::StringDataType &type, int from = -1, int to = -1);
    int retrieveStringData(QStringList &data, const WorldItem::StringDataType &type, int from = -1, int to = -1, bool set = false);
    int collectTriggers(WorldItem *w, QStringList &triggers, bool set = false);
    int collectPlatformClearTiles(WorldItem *w, QStringList &clrTiles, bool set = false);
    int collectMutableClassIds(WorldItem *w, QStringList &bgmcIds, bool set = false);
    int collectMutableIds(WorldItem *w, QStringList &mutString, bool set = false);

    QModelIndex validateModel();
    WorldItem* root();
    void clear();
    WorldItem* selectedItem() { return selectedDataItem; }
    QModelIndex indexOf(int row, int column, WorldItem* item);
    WorldItem* instanceOf(const QModelIndex &index);
    QVariant data(const QModelIndex &index, int role) const;
    void packDataBranch(const QModelIndex &index, int role, PlatzDataStream &stream) const;
    WorldItem* attachBranch(WorldItem *branchRoot, WorldItem *trunk, int row);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);
    Qt::DropActions supportedDropActions() const;

    bool insertRow (int row, const QModelIndex &parent = QModelIndex());
    bool insertRow (int row, WorldItem *child,  const QModelIndex &parent);
    bool insertRows (int row, int count, const QModelIndex &parent = QModelIndex());
    bool setItemData (const QModelIndex &index, const QMap<int, QVariant> &roles);
    QStringList mimeTypes () const;
    QMimeData* mimeData (const QModelIndexList &indexes) const;
    bool dropMimeData (const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    // Both of these delete all WorldItems and trigger scene clean up through connected views
    bool removeRow (int row, const QModelIndex &parent = QModelIndex());
    bool removeRows (int row, int count, const QModelIndex &parent = QModelIndex());
    void setDropState(PlatzDataModel::DropState state);
    bool removeBranch(const QModelIndex &parent);
    void sortBgOuters(Slice *slice);
    void cropWorldData(const QRectF &sliceBounds);
public slots:
    void setSelectedIndex(const QModelIndex &index);
signals:
    void rowDragDropped(const QModelIndex &parent, int start, int end);
    void selectedIndexChanged(const QModelIndex &index);
    void selectedIndexParentChanged(const QModelIndex &index);
    void currentIndexChanged(const QModelIndex &index);
    void bgoSortComplete(Slice *slice);
private:
    void updateGeometries(WorldItem *branchRoot);
    void updateCurrentIndex(const QModelIndex &parent, int start, int end);
    void selectDroppedItem();

    WorldItem *rootItem;
    WorldItem *selectedDataItem;
    PlatzDataModel::DropState dropState;
    QModelIndex selectedIndex;
    WorldItem *droppedItem;
};

#endif // PLATZDATAMODEL_H
