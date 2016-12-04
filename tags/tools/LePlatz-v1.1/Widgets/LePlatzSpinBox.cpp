
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

#include "LePlatzSpinBox.h"

LePlatzSpinBox::LePlatzSpinBox(QWidget *parent)
    : QSpinBox(parent), prevValue(1)
{
    setValue(prevValue);
    connect(this, SIGNAL(editingFinished()), this, SLOT(validateInput()));
    setMinimum(-32);
    setMaximum(32);
}

void LePlatzSpinBox::stepBy (int steps)
{
    int v = value();

    Q_ASSERT(v);
    
    if (v > 0) {
        if (steps > 0)
            v *= 2;
        else
            v /= 2;
    } else {
        if (steps > 0)
            v /= 2;
        else
            v *= 2;
    }
    v = qMin(v, 32);
    v = qMax(v, -32);

    if (!v && steps > 0)
        v = 1;
    else if (!v && steps < 0)
        v = -1;
    setValue(v);
    prevValue = v;
}

void LePlatzSpinBox::validateInput()
{
    int val = qAbs(value());

    if (!val || (val&(val-1))) // Power of 2 check
        setValue(prevValue);
    else
        prevValue = value();
}
