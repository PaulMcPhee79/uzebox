
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

#ifndef LEPLATZSPINBOX_H
#define LEPLATZSPINBOX_H

#include <QSpinBox>

class LePlatzSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    LePlatzSpinBox(QWidget *parent = 0);
    void stepBy (int steps);
public slots:
    void validateInput();
private:
    int prevValue;
};

#endif // LEPLATZSPINBOX_H
