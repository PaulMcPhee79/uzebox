#include "FocusSpinBox.h"

FocusSpinBox::FocusSpinBox(QWidget *parent)
    : QSpinBox(parent)
{
    setFocusPolicy(Qt::StrongFocus);
}

void FocusSpinBox::focusInEvent (QFocusEvent *event)
{
    emit receivedFocus();
    QSpinBox::focusInEvent(event);
}

void FocusSpinBox::focusOutEvent (QFocusEvent *event)
{
    emit focusLost();
    QSpinBox::focusOutEvent(event);
}
