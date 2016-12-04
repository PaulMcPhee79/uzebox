
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

#ifndef NEWPROJECT_H
#define NEWPROJECT_H

#include <QtGui/QDialog>
#include <QButtonGroup>
#include "Settings.h"

namespace Ui {
    class NewProject;
}

class NewProject : public QDialog {
    Q_OBJECT

public:
    NewProject(Settings *settings, QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    ~NewProject();
public slots:
    void displayDefaultHelp();
    void displayProjectNameHelp();
    void displayProjectFolderHelp();
    void displayArtFolderHelp();
    void displaySrcFolderHelp();
    void displaySliceWidthHelp();
    void displaySliceHeightHelp();
    void displaySpriteWidthHelp();
    void displaySpriteHeightHelp();
protected:
    QString folderFileDialog(const QString &initPath, const QString &title);
private slots:
    bool validateInput();
    void srcFolderFileDialog();
    void artFolderFileDialog();
    void projectFolderFileDialog();
    void createNewProject();
private:
    Ui::NewProject *ui;
    Settings *settings;
};

#endif // NEWPROJECT_H
