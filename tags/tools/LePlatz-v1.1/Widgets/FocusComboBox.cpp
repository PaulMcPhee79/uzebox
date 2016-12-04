#include "FocusComboBox.h"

FocusComboBox::FocusComboBox(QWidget *parent)
    : QComboBox(parent)
{
    setFocusPolicy(Qt::StrongFocus);
}

void FocusComboBox::focusInEvent (QFocusEvent *event)
{
    emit receivedFocus();
    QComboBox::focusInEvent(event);
}

void FocusComboBox::focusOutEvent (QFocusEvent *event)
{
    emit focusLost();
    QComboBox::focusOutEvent(event);
}
