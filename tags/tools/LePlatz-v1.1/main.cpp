
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


#include <QtGui/QApplication>
#include <QStringList>
#include <PlatzWin.h>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(Cursors);
    Q_INIT_RESOURCE(Icons);
    Q_INIT_RESOURCE(Misc);

    QApplication a(argc, argv);
    
    QString cmdLineProject;
    QStringList args = QApplication::arguments();

    if (args.length() > 1)
        cmdLineProject = args.at(1);
    PlatzWin w(cmdLineProject);
    w.show();
    return a.exec();
}
