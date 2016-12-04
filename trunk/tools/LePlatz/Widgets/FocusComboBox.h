#ifndef FOCUSCOMBOBOX_H
#define FOCUSCOMBOBOX_H

#include <QComboBox>

class FocusComboBox : public QComboBox
{
    Q_OBJECT

public:
    FocusComboBox(QWidget *parent = 0);

signals:
    void receivedFocus();
    void focusLost();
protected:
    void focusInEvent (QFocusEvent *event);
    void focusOutEvent (QFocusEvent *event);
};

#endif // FOCUSCOMBOBOX_H
