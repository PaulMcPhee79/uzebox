
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

#include "FocusLineEdit.h"

FocusLineEdit::FocusLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setFocusPolicy(Qt::StrongFocus);
}

void FocusLineEdit::focusInEvent (QFocusEvent *event)
{
    emit receivedFocus();
    QLineEdit::focusInEvent(event);
}

void FocusLineEdit::focusOutEvent (QFocusEvent *event)
{
    emit focusLost();
    QLineEdit::focusOutEvent(event);
}
