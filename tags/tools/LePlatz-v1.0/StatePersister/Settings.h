
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
        Settings(int maxRecentProjects = Platz::MAX_RECENT_PROJECTS);

        /*
         * Would like to make this interface private, but don't want to have to define a finite
         * number of contributors through friends. Instead, one 'parent' consumer will likely
         * obtain a Settings object and connect all other consumers.
         */

        int maxRecentProjects() { return mMaxRecentProjects; }

        // Project - get
        QString projectName();
        const QString& projectPath() { return mProjectPath; }
        const QString& imageFormat() { return mImageFormat; }
        const QString& srcFolder() { return mSrcFolder; }
        const QString& slicePath() { return mSlicePath; }
        const QString& tilePath() { return mTilePath; }
        const QString& mapPath() { return mMapPath; }
        const QString& animPath() { return mAnimPath; }
        const QString& makefilePath() { return mMakefilePath; }
        const QString& hexfilePath() { return mHexfilePath; }
        const QString& platzfilePath() { return mPlatzfilePath; }
        QSize sliceSize() { return mSliceSize; }
        QSize spriteSize() { return mSpriteSize; }
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
        void setSliceSize(const QSize &size);
        void setSliceSize(int wid, int hgt);
        void setSpriteSize(const QSize &size);
        void setSpriteSize(int wid, int hgt);

        // LePlatz - get
        const QString& makeExePath() { return mMakeExePath; }
        const QString& emuExePath() { return mEmuExePath; }
        const QStringList& recentProjects() { return mRecentProjects; }

        // LePlatz - set
        void setMakeExePath(const QString &path);
        void setEmuExePath(const QString &path);
        void addRecentProject(const QString &project);
        void addRecentProjects(const QStringList &projects);
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
        void sliceSizeChanged(const QSize &size);
        void spriteSizeChanged(const QSize &size);
        void makeExePathChanged(const QString &path);
        void emuExePathChanged(const QString &path);
        void recentProjectsChanged(const QStringList &recentProjects);
    public slots:
        void resetProjectSettings();
        bool loadSettings(const QString &path);
        bool loadProject(const QString &path, PlatzDataModel *model);
        bool loadLePlatzSettings(QByteArray &winGeometry, QByteArray &winState);
        bool saveProject(WorldItem *root);
        bool saveProject(const QString &path, WorldItem *root);
        bool saveLePlatzSettings(const QByteArray &winGeometry, const QByteArray &winLayout);
        int showNewProjectDialog(QWidget *parent);
        int showEditSettingsDialog(QWidget *parent);
    private:
        QString resolvePath(const QString &path);
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
        QSize mSliceSize;
        QSize mSpriteSize;
        // LePlatz
        QString mMakeExePath;
        QString mEmuExePath;
        QStringList mRecentProjects;
};











#if 0

// Add support for QSettings to save/restore gui layout
class Settings : public QObject
{
    Q_OBJECT

    class WorldSettings {
    public:
        enum WorldSettingsXmlOrder {
            Version,
            Settings,
            SliceSize,
            LargestSprite,
            WorldPath,
            SlicePath,
            TilePath,
            MapPath,
            AnimPath,
            PlatzPath,
            SourcePath,
            MakefilePath,
            HexfilePath
        };

        WorldSettings();

        bool saveWorldSettings(const QString &filename);
        bool loadWorldSettings(const QString &filename);

        QSize sliceSize;
        QSize largestSprite;    // So LePlatz can determine slice seam collision bgs
        QString worldPath;
        QString slicePath;
        QString tilePath;
        QString mapPath;
        QString animPath;
        QString platzPath;
        QString srcPath;        // Have some javadoc-esque tag for mutable/trigger defines in source files
        QString makefilePath;
        QString hexfilePath;
    private:
        QXmlStreamWriter writer;
        QXmlStreamReader reader;
    };

    class LePlatzSettings {
    public:
        enum LePlatzSettingsXmlOrder {
            Version,
            Settings,
            SliceSize,
            LargestSprite,
            MakeExePath,
            EmuExePath,
            RecentProjects,
            Project
        };

        LePlatzSettings();

        bool saveLePlatzSettings();
        bool loadLePlatzSettings();

        QSize sliceSize;
        QSize largestSprite;    // So LePlatz can determine slice seam collision bgs
        QString makeExePath;
        QString emuExePath;
        QStringList recentProjects;
    private:
        QXmlStreamWriter writer;
        QXmlStreamReader reader;
    };

public:
    Settings();
    virtual ~Settings();

    enum SettingsType {
        Auto,       // Have class choose most likely candidate
        LePlatz,
        World
    };

    void setSliceSizeWorld(const QSize &size) { ws.sliceSize = size; emit (largestSpriteChanged(largestSprite())); }
    void setLargestSpriteLePlatz(const QSize &size) { lps.largestSprite = size; }
    void setLargestSpriteWorld(const QSize &size) { ws.largestSprite = size; }
    void setMakeExePath(const QString &path) { lps.makeExePath = path; emit (makeExePathChanged(makeExePath())); }
    void setEmuExePath(const QString &path) { lps.emuExePath = path; emit (emuExePathChanged(emuExePath())); }

    void setArtPath(const QString &path) {}
    void setSourcePath(const QString &path) {}

    void setWorldPath(const QString &path) { ws.worldPath = path; emit (worldPathChanged(worldPath())); }
    void setPlatzPath(const QString &path) { ws.platzPath = path; emit (platzPathChanged(platzPath())); }
    void setSourcePath(const QString &path) { ws.srcPath = path; emit (sourcePathChanged(srcPath())); }
    void setMakefilePath(const QString &path) { ws.makefilePath = path; emit (makefilePathChanged(makefilePath())); }
    void setHexfilePath(const QString &path) { ws.hexfilePath = path; emit (hexfilePathChanged(hexfilePath())); }
    void setSlicePath(const QString &path) { ws.slicePath = path; emit (slicePathChanged(slicePath())); }
    void setTilePath(const QString &path) { ws.tilePath = path; emit (tilePathChanged(tilePath())); }
    void setMapPath(const QString &path) { ws.mapPath = path; emit (mapPathChanged(mapPath())); }
    void setAnimPath(const QString &path) { ws.animPath = path; emit (animPathChanged(animPath())); }
    const QString& worldPath() const { return ws.worldPath; }
    const QString& platzPath() const { return ws.platzPath; }
    const QString& srcPath() const { return ws.srcPath; }
    const QString& makefilePath() const { return ws.makefilePath; }
    const QString& hexfilePath() const { return ws.hexfilePath; }
    const QString& makeExePath() const { return lps.makeExePath; }
    const QString& emuExePath() const { return lps.emuExePath; }
    const QString& slicePath() const { return ws.slicePath; }
    const QString& tilePath() const { return ws.tilePath; }
    const QString& mapPath() const { return ws.mapPath; }
    const QString& animPath() const { return ws.animPath; }
    void addRecentProject(const QString &path);
    const QSize& sliceSize(const SettingsType &type = Auto) const;
    const QSize& largestSprite(const SettingsType &type = Auto) const;
    QString applicationPath() const;
signals:
    void sliceSizeChanged(const QSize &size);
    void largestSpriteChanged(const QSize &size);
    void makeExePathChanged(const QString &path);
    void emuExePathChanged(const QString &path);
    void worldPathChanged(const QString &path);
    void platzPathChanged(const QString &path);
    void sourcePathChanged(const QString &path);
    void makefilePathChanged(const QString &path);
    void hexfilePathChanged(const QString &path);
    void slicePathChanged(const QString &path);
    void tilePathChanged(const QString &path);
    void mapPathChanged(const QString &path);
    void animPathChanged(const QString &path);
public slots:
    void show(QWidget *parent);
    void uiFinished(int result);
    bool loadSettings(QString worldSettingsPath);
    bool saveSettings(QString worldSettingsPath);
    void cleanup();
private:
    SettingsDialog *ui;
    LePlatzSettings lps;
    WorldSettings ws;
};
#endif

#endif // SETTINGS_H
