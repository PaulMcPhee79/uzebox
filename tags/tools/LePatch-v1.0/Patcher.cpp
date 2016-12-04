
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

#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include <Interpreter.h>
#include "Patcher.h"

Patcher::Patcher(QObject *parent, int timeout)
    : QObject(parent)
{
    procPatch = new QProcess(this);
    connect(procPatch, SIGNAL(started()), this, SLOT(patchStarted()));
    connect(procPatch, SIGNAL(error(QProcess::ProcessError)), this, SLOT(patchError(QProcess::ProcessError)));
    connect(&patchTmr, SIGNAL(timeout()), this, SLOT(patchTimeout()));
    connect(procPatch, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(patchApplied(int,QProcess::ExitStatus)));

    // Timer
    patchTmr.setSingleShot(true);
    patchTmr.setInterval(timeout);
}

void Patcher::applyPatch(QString tool, QString oldFile, QString deltaFile, QString newFile)
{
    QStringList arguments;
    arguments << "-d" << "-s" << oldFile << deltaFile << newFile;
    procPatch->setProcessChannelMode(QProcess::MergedChannels);
    procPatch->setReadChannel(QProcess::StandardOutput);
    procPatch->setWorkingDirectory(QCoreApplication::applicationDirPath());
    emit patchStarted(deltaFile);
    procPatch->start(tool, arguments);
}

void Patcher::patchStarted()
{
    if (patchTmr.isActive())
        patchTmr.stop();
    patchTmr.start();
}

void Patcher::patchError(QProcess::ProcessError error)
{
    Q_ASSERT(interpreter);

    if (patchTmr.isActive())
        patchTmr.stop();
    interpreter->exit(1);
}

void Patcher::patchTimeout()
{
    Q_ASSERT(interpreter);

    if (procPatch && procPatch->isOpen())
        procPatch->close();
}

void Patcher::patchApplied(int exitCode, QProcess::ExitStatus)
{
    Q_ASSERT(interpreter);

    if (patchTmr.isActive()) {
        patchTmr.stop();
        interpreter->exit(exitCode);
    } else {
        interpreter->exit(1);
    }
}

Patcher::~Patcher()
{
    if (procPatch && procPatch->isOpen())
        procPatch->close();
    delete procPatch;
}
