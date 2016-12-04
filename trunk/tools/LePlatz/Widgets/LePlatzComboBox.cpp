
#include <QKeyEvent>
#include "LePlatzComboBox.h"

LePlatzComboBox::LePlatzComboBox(QWidget *parent)
    : QComboBox(parent), fkeyProxy(0)
{
}

void LePlatzComboBox::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
        case Qt::Key_F1:
        case Qt::Key_F2:
        case Qt::Key_F3:
        case Qt::Key_F4:
        case Qt::Key_F5:
        case Qt::Key_F6:
        case Qt::Key_F7:
            if (fkeyProxy) {
                fkeyProxy->publicKeyPressEvent(e);
                return;
            }
            break;
        default:
            break;
    }
    QComboBox::keyPressEvent(e);
}
