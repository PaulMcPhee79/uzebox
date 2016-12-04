
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

#ifndef PLATZWIN_H
#define PLATZWIN_H

#include <QtGui/QMainWindow>
#include <QProcess>
#include <QAction>
#include <QProgressBar>
#include <QPlainTextEdit>
#include <QComboBox>
#include <PlatzEnums.h>
#include <Settings.h>
#include <PlatzDataModel.h>
#include <BgPlatformPath.h>
#include <BgPlatform.h>

namespace Ui
{
    class PlatzWin;
}

using namespace Platz;

class PlatzWin : public QMainWindow
{
    Q_OBJECT

public:
    PlatzWin(const QString &cmdLineProject, QWidget *parent = 0);
    ~PlatzWin();
public slots:
    void publicKeyPressEvent(QKeyEvent *e);

private slots:
    void newProject();
    bool loadProject(const QString &path);
    bool loadProject(QAction *action);
    bool saveProject(const QString &path);
    bool closeProject();
    bool close();
    bool initLoadProject();
    void initSaveProject();
    bool initSaveAsProject();
    bool prepareToClose();
    void removeSlice();
    void insertSlice();
    void reloadResources();
    void loadResources(QString &msg);
    bool loadBackground();
    void compileWorld();
    void buildSource();
    void playInEmulator();
    void makeOutputReady();
    void emuOutputReady();
    void makeFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void emuFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void makeError(QProcess::ProcessError error);
    void emuError(QProcess::ProcessError error);
    void setToolboxOptions(QAction *action);
    void setInteractionMode(QAction *action);
    void showSettings();
    void setProjectName(const QString &name);
    void setProjectPath(const QString &path) { projectPath = path; }
    void setSlicePath(const QString &path) { slicePath = path; }
    void setTilePath(const QString &path) { tilePath = path; }
    void setMapPath(const QString &path) { mapPath = path; }
    void setAnimPath(const QString &path) { animPath = path; }
    void setMakefilePath(const QString &path) { makefilePath = path; }
    void setHexfilePath(const QString &path) { hexfilePath = path; }
    void setPlatzfilePath(const QString &path) { platzfilePath = path; }
    void setSrcFolder(const QString &folder) { srcFolder = folder; }
    void setMakeExePath(const QString &path) { makeExePath = path; }
    void setEmuExePath(const QString &path) { emuExePath = path; }
    void setVideoMode(int vmode);
    void setSliceSize(const QSize &size);
    void setSpriteSize(const QSize &size) { spriteSize = size; }
    void setTileSize(const QSize &size) { tileSize = size; }
    void setOffsetY(int offset) { offsetY = offset; }
    void setUpdatesUrl(const QString &url) { updatesUrl = url; }
    void clearBgoCheckBoxes();
    void updateBgoToolboxAttributes();
    void updateBgiToolboxAttributes();
    void updateBgmToolboxAttributes();
    void updatePlatformToolboxAttributes();
    void populateCombo(const QStringList &ids, QComboBox *cbo);
    void updateDetailDataDisplay(const QModelIndex &index);
    void updateDetailDataDisplay(const WorldItem *item = 0);
    void updateDetailImageDisplay(const QImage &img = QImage());
    void aboutLePlatz();
    void aboutPlatz();
    void aboutUzebox();
    InteractionMode fkeyToIntMode(int fkey);
    void updateRecentProjects(const QStringList &recentProjects);
    void toggleBgoTriggerMode();
    void flagUnsavedChanges() { unsavedChanges = true; }
    void replicateSlice();
    void toggleSelectedSliceLock();
    void sortBgoOrder();
    void setSnapToResolutionX(QAction* action);
    void setSnapToResolutionY(QAction* action);
    int findReplaceSrcDefines();
    void updateTreeviewPluses(Slice *slice);
    void checkForUpdates();
    void progressUpdate(const QString &fileName, int bytesRead, int totalBytes, int fileIndex, int totalFiles);
    void downloadComplete(bool error, const QString &errMsg);
    void installUpdates();

signals:
    void zoomIn(int delta = 120);
    void zoomOut(int delta = -120);
    void bgoAttributesChanged(int flags);
    void bgoTriggerChanged(const QString &trigId, const QString &trigOrientation);
    void bgiTileChanged(int tile);
    void bgiFlagsChanged(int flags);
    void bgmMutableStringChanged(const QString &s);
    void bgmClassChanged(const QString &bgmc);
    void bgmCustomPayloadChanged(const Platz::MutablePayload &payload);
    void platformStyleChanged(BgPlatform::PlatformStyle style);
    void platformAxisChanged(BgPlatformPath::PlatformAxis axis);
    void platformClearTileChanged(const QString &tile);
    void platformVelocityChanged(int vel);
    void imModeTriggered(QAction* action);
protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);
    void changeEvent (QEvent *event);
private slots:
    void toggleBgmi(bool checked);
    void toggleBgmc(bool checked);
    void toggleBgmcCombo(bool enable);
private:
    bool queryUnsavedChanges();
    void PrintProcError(QProcess::ProcessError error, QPlainTextEdit *txtEdit);
    Ui::PlatzWin *ui;
    PlatzDataModel *model;
    QProcess *procMake;
    QProcess *procEmu;
    Settings *settings;
    QString projectName;
    QString projectPath;
    QString slicePath;
    QString tilePath;
    QString mapPath;
    QString animPath;
    QString srcFolder;
    QString makefilePath;
    QString hexfilePath;
    QString platzfilePath;
    QString makeExePath;
    QString emuExePath;
    int videoMode;
    QSize sliceSize;
    QSize spriteSize;
    QSize tileSize;
    int offsetY;
    bool activeProject;
    QMenu *recProjMenu;
    QActionGroup *recProjActionGrp;
    QActionGroup *snapToYGrp;
    QActionGroup *snapToXGrpVMode2;
    QActionGroup *snapToXGrpVMode3;
    bool bgoTrigger;
    bool unsavedChanges;
    QProgressBar *updateProgress;
    QString updatesUrl;
    bool updateInProgress;
};

#endif // PLATZWIN_H
