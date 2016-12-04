
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

#include <QApplication>
#include <QPointer>
#include <QDir>
#include <QFileInfo>
#include <PlatzWriter.h>
#include <PlatzReader.h>
#include "NewProject.h"
#include "SettingsDialog.h"
#include "Settings.h"

static const QString UNSET = "";

Settings::Settings(int maxRecentProjects)
    : mMaxRecentProjects(maxRecentProjects), mSliceSize(QSize(0,0)),
        mSpriteSize(QSize(0,0))
{
}

int Settings::showNewProjectDialog(QWidget *parent)
{
    QPointer<NewProject> np = new NewProject(this, parent);
    int result = np->exec();
    delete np;
    return result;
}

int Settings::showEditSettingsDialog(QWidget *parent)
{
    QPointer<SettingsDialog> sd = new SettingsDialog(this, parent);
    int result = sd->exec();
    delete sd;
    return result;
}

void Settings::resetProjectSettings()
{
    setProjectPath("LePlatz");
    setSliceSize(0, 0);
    setSpriteSize(0, 0);
    setArtFolder("");
    setRelativeSrcFolder("");
}

QString Settings::resolvePath(const QString &path)
{
    QDir dir(path);

    if (dir.isRelative())
        dir.setPath(QFileInfo(mProjectPath).absoluteDir().absolutePath() + "/" + QDir(mProjectPath).relativeFilePath(path));
    return dir.path();
}

QString Settings::projectName()
{
    QString name = QFileInfo(mProjectPath).fileName(), xml = ".xml";

    if (name.endsWith(xml))
        name.chop(xml.length());
    return name;
}

void Settings::setProjectPath(const QString &path) {
    mProjectPath = path;
    emit projectPathChanged(path);
    emit projectNameChanged(projectName());
}

void Settings::setSliceSize(const QSize &size) {
    mSliceSize = size;
    emit sliceSizeChanged(mSliceSize);
}

void Settings::setSliceSize(int wid, int hgt) {
    mSliceSize = QSize(wid, hgt);
    emit sliceSizeChanged(mSliceSize);
}

void Settings::setSpriteSize(const QSize &size) {
    mSpriteSize = size;
    emit spriteSizeChanged(mSpriteSize);
}

void Settings::setSpriteSize(int wid, int hgt) {
    mSpriteSize = QSize(wid, hgt);
    emit spriteSizeChanged(mSpriteSize);
}

void Settings::setArtFolder(const QString &folder)
{
    setSlicePath(folder + "/slices/");
    setTilePath(folder + "/tiles" + mImageFormat);
    setMapPath(folder + "/maps" + mImageFormat);
    setAnimPath(folder + "/animations" + mImageFormat);
}

void Settings::setRelativeSrcFolder(const QString &folder)
{
    mSrcFolder = folder;
    emit srcFolderChanged(resolvePath(folder));
    setMakefilePath(mSrcFolder + "/default/Makefile");
    setHexfilePath(mSrcFolder + "/default/" + projectName() + ".hex");
    setPlatzfilePath(mSrcFolder + "/data/platz.levels.inc");
}

void Settings::setSrcFolder(const QString &folder)
{
    mSrcFolder = folder;
    emit srcFolderChanged(resolvePath(folder));
}

void Settings::setSlicePath(const QString &path)
{
    mSlicePath = path;
    emit slicePathChanged(resolvePath(path));
}

void Settings::setTilePath(const QString &path)
{;
    mTilePath = path;
    emit tilePathChanged(resolvePath(path));
}

void Settings::setMapPath(const QString &path)
{
    mMapPath = path;
    emit mapPathChanged(resolvePath(path));
}

void Settings::setAnimPath(const QString &path)
{
    mAnimPath = path;
    emit animPathChanged(resolvePath(path));
}

void Settings::setMakefilePath(const QString &path)
{
    mMakefilePath = path;
    emit makefilePathChanged(resolvePath(path));
}

void Settings::setHexfilePath(const QString &path)
{
    mHexfilePath = path;
    emit hexfilePathChanged(resolvePath(path));
}

void Settings::setPlatzfilePath(const QString &path)
{
    mPlatzfilePath = path;
    emit platzfilePathChanged(resolvePath(path));
}

void Settings::setMakeExePath(const QString &path) {
    mMakeExePath = path;
    emit makeExePathChanged(mMakeExePath);
}
void Settings::setEmuExePath(const QString &path) {
    mEmuExePath = path;
    emit emuExePathChanged(path);
}

void Settings::addRecentProject(const QString &project)
{
    QString path = QFileInfo(project).absoluteFilePath();

    if (mRecentProjects.contains(path))
        mRecentProjects.removeAt(mRecentProjects.indexOf(path));
    mRecentProjects.insert(0, path);

    if (mRecentProjects.count() > mMaxRecentProjects)
        mRecentProjects.removeLast();
    emit recentProjectsChanged(mRecentProjects);
}

void Settings::addRecentProjects(const QStringList &projects)
{
    int addCount = projects.count();

    if (addCount > mRecentProjects.count()) {
        mRecentProjects.clear();
    } else {
        for (int i = 0; i < addCount; i++)
            mRecentProjects.removeLast();
    }

    while (addCount && (mRecentProjects.count() < mMaxRecentProjects)) {
        mRecentProjects.insert(0, projects.at(addCount-1));
        --addCount;
    }
    emit recentProjectsChanged(mRecentProjects);
}

bool Settings::loadLePlatzSettings(QByteArray &winGeometry, QByteArray &winLayout)
{
    PlatzReader reader(this);
    return reader.loadLePlatzSettings(QApplication::applicationDirPath() + "/LePlatzSettings.xml", winGeometry, winLayout);
}

bool Settings::saveLePlatzSettings(const QByteArray &winGeometry, const QByteArray &winLayout)
{
    PlatzWriter writer(this);
    return writer.saveLePlatzSettings(QApplication::applicationDirPath() + "/LePlatzSettings.xml", winGeometry, winLayout);
}

bool Settings::loadProject(const QString &path, PlatzDataModel *model)
{
    if (!model)
        return false;
    setProjectPath(path);
    model->clear();
    PlatzReader reader(this, model);
    bool result = reader.loadProject(path, PlatzReader::ReadWorld);

    if (!result)
        setProjectPath("");
    return result;
}

bool Settings::loadSettings(const QString &path)
{
    setProjectPath(path);

    PlatzReader reader(this);
    bool result = reader.loadProject(path, PlatzReader::ReadSettings);

    if (!result)
        setProjectPath("");
    return result;
}

bool Settings::saveProject(WorldItem *root)
{
    return saveProject(mProjectPath, root);
}

bool Settings::saveProject(const QString &path, WorldItem *root)
{
    if (!root)
        return false;
    PlatzWriter writer(this, root);
    bool result = writer.saveWorld(path);

    if (result)
        setProjectPath(path);
    return result;
}
