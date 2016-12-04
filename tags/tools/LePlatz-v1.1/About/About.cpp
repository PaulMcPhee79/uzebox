
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

#include "About.h"
#include "ui_About.h"

About::About(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
    m_ui(new Ui::About)
{
    m_ui->setupUi(this);
    m_ui->lblAboutLogo->setText("");
    connect(m_ui->pbOk, SIGNAL(pressed()), this, SLOT(close()));
}

About::~About()
{
    delete m_ui;
}

void About::setTitle(const QString &title)
{
    m_ui->lblAboutTitle->setText(title);
}

void About::setText(const QString &text)
{
    m_ui->lblAboutText->setText(text);
}

void About::setUrl(const QString &url)
{
    m_ui->lblUrl->setText(url);
}

void About::setPixmap(const QPixmap &pixmap)
{
    m_ui->lblAboutLogo->setPixmap(pixmap);
}
