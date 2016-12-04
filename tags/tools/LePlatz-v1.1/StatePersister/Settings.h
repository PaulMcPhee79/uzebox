
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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QStringList>
#include <QSize>
#include <WorldItem.h>
#include <PlatzDataModel.h>

// Add support for QSettings to save/restore gui layout
class Settings : public QObject
{
    Q_OBJECT

    public:

        static const int VMODE2_SCREEN_TILES_V;
        static const int VMODE3_SCREEN_TILES_V;
        static const QString DEFAULT_UPDATES_URL;

        Settings(int maxRecentProjects = Platz::MAX_RECENT_PROJECTS);

        /*
         * Would like to make this interface private, but don't want to have to define a finite
         * number of contributors through friends. Instead, one 'parent' consumer will likely
         * obtain a Settings object and connect all other consumers.
         */

        int maxRecentProjects() { return mMaxRecentProjects; }

        // Project - get
        QString resolvePath(const QString &path);
        QString projectName();
        const QString& projectPath() const { return mProjectPath; }
        const QString& imageFormat() const { return mImageFormat; }
        const QString& srcFolder() const { return mSrcFolder; }
        const QString& slicePath() const { return mSlicePath; }
        const QString& tilePath() const { return mTilePath; }
        const QString& mapPath() const { return mMapPath; }
        const QString& animPath() const { return mAnimPath; }
        const QString& makefilePath() const { return mMakefilePath; }
        const QString& hexfilePath() const { return mHexfilePath; }
        const QString& platzfilePath() const { return mPlatzfilePath; }
        int videoMode() const { return mVideoMode; }
        QSize tileSize() const;
        QSize sliceSize() const { return mSliceSize; }
        QSize spriteSize() const { return mSpriteSize; }
        int offsetY() const { return mOffsetY; }

        // Project - set
        void setProjectPath(const QString &path);
        void setImageFormat(const QString &format) { mImageFormat = format; }
        void setArtFolder(const QString &folder);
        void setRelativeSrcFolder(const QString &folder);
        void setSrcFolder(const QString &folder);
        void setSlicePath(const QString &path);
        void setTilePath(const QString &path);
        void setMapPath(const QString &path);
        void setAnimPath(const QString &path);
        void setMakefilePath(const QString &path);
        void setHexfilePath(const QString &path);
        void setPlatzfilePath(const QString &path);
        void setVideoMode(int mode);
        void setSliceSize(const QSize &size);
        void setSliceSize(int wid, int hgt);
        void setSpriteSize(const QSize &size);
        void setSpriteSize(int wid, int hgt);
        void setOffsetY(int offset);

        // LePlatz - get
        const QString& makeExePath() { return mMakeExePath; }
        const QString& emuExePath() { return mEmuExePath; }
        const QString& updatesUrl() { return mUpdatesUrl; }
        const QStringList& recentProjects() { return mRecentProjects; }
        const QColor& canvasColor() { return mCanvasColor; }

        // LePlatz - set
        void setMakeExePath(const QString &path);
        void setEmuExePath(const QString &path);
        void setUpdatesUrl(const QString &url);
        void addRecentProject(const QString &project);
        void addRecentProjects(const QStringList &projects);
        void setCanvasColor(const QColor &color);
    signals:
        // Consumers connect to these. Contributors interact through public interface.
        void projectNameChanged(const QString &name);
        void projectPathChanged(const QString &path);
        void slicePathChanged(const QString &path);
        void tilePathChanged(const QString &path);
        void mapPathChanged(const QString &path);
        void animPathChanged(const QString &path);
        void srcFolderChanged(const QString &folder);
        void makefilePathChanged(const QString &path);
        void hexfilePathChanged(const QString &path);
        void platzfilePathChanged(const QString &path);
        void videoModeChanged(int mode);
        void sliceSizeChanged(const QSize &size);
        void spriteSizeChanged(const QSize &size);
        void makeExePathChanged(const QString &path);
        void emuExePathChanged(const QString &path);
        void updatesUrlChanged(const QString &url);
        void recentProjectsChanged(const QStringList &recentProjects);
        void canvasColorChanged(const QColor &color);
        void tileSizeChanged(const QSize &size);
        void offsetYChanged(int offset);
    public slots:
        void resetProjectSettings();
        bool loadSettings(const QString &path, QString *errMsg = 0);
        bool loadProject(const QString &path, PlatzDataModel *model, QString *errMsg = 0);
        bool loadLePlatzSettings(QByteArray &winGeometry, QByteArray &winState, QString *errMsg = 0);
        bool saveProject(WorldItem *root);
        bool saveProject(const QString &path, WorldItem *root);
        bool saveLePlatzSettings(const QByteArray &winGeometry, const QByteArray &winLayout);
        int showNewProjectDialog(QWidget *parent);
        int showEditSettingsDialog(QWidget *parent);
    private:
        // General
        int mMaxRecentProjects;
        // Project
        QString mProjectPath;
        QString mImageFormat;
        QString mSlicePath;
        QString mTilePath;
        QString mMapPath;
        QString mAnimPath;
        QString mSrcFolder;
        QString mMakefilePath;
        QString mHexfilePath;
        QString mPlatzfilePath;
        int mVideoMode;
        QSize mSliceSize;
        QSize mSpriteSize;
        // LePlatz
        QString mMakeExePath;
        QString mEmuExePath;
        QString mUpdatesUrl;
        QStringList mRecentProjects;
        QColor mCanvasColor;
        int mOffsetY;
};

#endif
