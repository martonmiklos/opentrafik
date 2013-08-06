#include "barcodelineedit.h"
#include <QDebug>

BarcodeLineEdit::BarcodeLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
}


void BarcodeLineEdit::keyPressEvent(QKeyEvent *event)
{
    if (/*event->nativeScanCode() == 19 || */event->nativeScanCode() == 11) { // ö -> 0
        QKeyEvent fakeEvent(event->type(), Qt::Key_0, event->modifiers(), "0", false, 1);
        QLineEdit::keyPressEvent(&fakeEvent);
    } else {
        QLineEdit::keyPressEvent(event);
    }
}
