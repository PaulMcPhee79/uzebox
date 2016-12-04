
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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui
{
    class SettingsDialog;
}

class Settings;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    enum PathType {
        FilePath,
        DirPath
    };

    SettingsDialog(Settings *settings, QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    ~SettingsDialog();
//signals:
    //void worldPathTextChanged(const QString &text, int index = 2);
public slots:
    void okSettings();
    void applySettings();
    void settingSelected(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void settingPsItemSelected(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void settingLpsItemSelected(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void showSettingsDialog(QString &path, QString openPath, const PathType &type = FilePath, const QString &filter = "*.*");
private slots:
    void updateTrees(const QString &text);
    void pathFileDialog();
private:
    Ui::SettingsDialog *ui;
    Settings *settings;
};

#endif // SETTINGSDIALOG_H
