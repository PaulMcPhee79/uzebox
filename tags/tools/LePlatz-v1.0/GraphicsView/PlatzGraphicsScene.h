
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

#ifndef PLATZGRAPHICSSCENE_H
#define PLATZGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <PlatzDataModel.h>
#include <BgPlatformPath.h>
#include <BgPlatform.h>
#include <PlatzEnums.h>
#include <BgInner.h>
#include "PlatzGraphicsItem.h"

using namespace Platz;

class PlatzGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    
    enum DrawState { 
        NoDraw, 
        BeginDraw, 
        Drawing 
    };

    static QList<QPen> pens;
    static QList<QBrush> brushes;
    static void initSceneDrawTools();
    
    PlatzGraphicsScene(QObject *parent);
    ~PlatzGraphicsScene();
    Platz::InteractionMode interactionMode();
    const InteractionMode& mode();
    void setMode(const InteractionMode &im);
    void setModel(PlatzDataModel *model);
    PlatzDataModel* model();
    const QList<QGraphicsItem*>& selectedPlatzItems();
    bool isSelectionVisible();
    BgInner* mutableParent() { return mutParent; }
public slots:
    void showSelection(void);
    void cancelSelection(void);
    void setSelection(const QModelIndex &index);
    bool removeSelection(QGraphicsItem *selItem);
    void appendSelection(QGraphicsItem *selItem);
    void appendSelection(const QList<QGraphicsItem*> &selItem);
    void selectAll();
    void hideOuterJoin(const InteractionMode &mode);
    void clearOuterJoin();
    void hideSelection();
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
    void setBgiFlags(int flags) { bgiFlags = flags; }
    void setBgoFlags(int flags) { bgoFlags = flags; }
    void setBgoTriggers(int id, int orientation) { trigId = id; trigOrientation = orientation; }
    void setBgiTile(int tile) { bgiTile = tile; }
    void setPlatformStyle(BgPlatform::PlatformStyle style) { platformStyle = style; }
    void setPlatformAxis(BgPlatformPath::PlatformAxis axis) { platformAxis = axis; }
    void setPlatformClearTile(const QString &tile) { platformClrTile = tile; }
    void setPlatformVelocity(int vel) { platformVel = vel; }
    void setPayloadToCustom(bool useCustom) { useCustomPayload = useCustom; }
    void setCustomPayload(const Platz::MutablePayload &payload) { customPayload = payload; }
    void emitIndexChanges(const WorldItem *item);
    void setMutableParent(BgInner *mp) { mutParent = mp; }
    void setSnapToResolution(int resolution);
signals:
    void selectionChanged(const QModelIndex &index);
    void itemDrawn(WorldItem *item);
    void interactionModeChanged(Platz::InteractionMode mode);
    //void currentItemChanged(const WorldItem *item);
    void currentItemDataChanged(const WorldItem *item);
    void currentTileIndexChanged(int index);
    void currentMapIndexChanged(int index);
    void currentAnimIndexChanged(int index);
protected:
    void makeSelection(bool emitSignal = true);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
private:
    void clampToScene(QPointF &p);
    void clampToGrid(QPointF &p);
    QRectF bgOutline;
    DrawState state;
    PlatzGraphicsItem *item;
    PlatzGraphicsItem *sel;
    Platz::InteractionMode sceneMode;
    QList<QGraphicsItem*> selection;
    QList<PlatzGraphicsItem*> join;
    PlatzDataModel *dataModel;
    int row;
    QModelIndex parent;
    bool selectionHidden;
    // Bg attributes (connected to main window's toolbox)
    int bgiFlags;
    int bgoFlags;
    int trigId;
    int trigOrientation;
    int bgiTile; // Doubles as mutable id due to Platz level format
    BgPlatform::PlatformStyle platformStyle;
    BgPlatformPath::PlatformAxis platformAxis;
    QString platformClrTile;
    int platformVel;
    bool useCustomPayload;
    Platz::MutablePayload customPayload;
    BgInner *mutParent;
    int snapToResolution;
};

#endif // PLATZGRAPHICSSCENE_H
