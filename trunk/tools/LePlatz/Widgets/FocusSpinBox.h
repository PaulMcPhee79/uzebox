#ifndef FOCUSSPINBOX_H
#define FOCUSSPINBOX_H

#include <QSpinBox>

class FocusSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    FocusSpinBox(QWidget *parent = 0);

signals:
    void receivedFocus();
    void focusLost();
protected:
    void focusInEvent (QFocusEvent *event);
    void focusOutEvent (QFocusEvent *event);
};

#endif // FOCUSSPINBOX_H
