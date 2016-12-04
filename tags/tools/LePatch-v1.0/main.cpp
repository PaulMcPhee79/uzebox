
/*
 *  LePatch - A patch tool for LePlatz
 *  Copyright (C) 2010 Paul McPhee
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

#include <QtGui/QApplication>
#include <QStringList>
#include "LePatchDlg.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList args = QApplication::arguments();
    LePatchDlg w;
    int it = 1;
    QString version("1.0"), updateScript("");

    while (it < args.count()) {
        if (args.at(it) == "-v") {
            if (++it < args.count()) {
                version = args.at(it);
                break;
            }
        }
        ++it;
    }

    if (updateScript.isEmpty())
        updateScript = "LePlatz-Updates.xml";
    w.show();
    w.setUpdateScript(updateScript);
    w.setVersion(version);
    return a.exec();
}
