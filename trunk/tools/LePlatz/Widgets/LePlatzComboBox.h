#ifndef LEPLATZCOMBOBOX_H
#define LEPLATZCOMBOBOX_H

#include <QComboBox>
#include <PlatzWin.h>

// The purpose of this class is to route F1-F7 keypresses (LePlatz's main toolbox hotkeys) to
// the main window. Otherwise (at least on Windows), the combobox expands/retracts with some F# keys.
class LePlatzComboBox : public QComboBox
{
    Q_OBJECT

public:
    LePlatzComboBox(QWidget *parent = 0);
    void setFkeyProxy(PlatzWin *pwin) { fkeyProxy = pwin; }
protected:
    void keyPressEvent(QKeyEvent *e);
private:
    PlatzWin *fkeyProxy;
};

#endif // LEPLATZCOMBOBOX_H
