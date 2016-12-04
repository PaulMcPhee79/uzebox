
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

#ifndef PATCHER_H
#define PATCHER_H

#include <QObject>
#include <QProcess>
#include <QTimer>

class Interpreter;

class Patcher : public QObject
{
    Q_OBJECT

public:
    Patcher(QObject *parent, int timeout);
    ~Patcher();

    void setInterpreter(Interpreter *it) { interpreter = it; }

public slots:
    void applyPatch(QString tool, QString oldFile, QString deltaFile, QString newFile);

signals:
    void patchStarted(const QString &patch);
    void patchComplete(bool error);

private slots:
    void patchStarted();
    void patchError(QProcess::ProcessError error);
    void patchApplied(int exitCode, QProcess::ExitStatus);
    void patchTimeout();
private:
    QProcess *procPatch;
    QTimer patchTmr;
    Interpreter *interpreter;
};

#endif // PATCHER_H
