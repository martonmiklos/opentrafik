#ifndef DIALOGCORRECTOR_H
#define DIALOGCORRECTOR_H

#include <QDialog>
#include "common.h"


namespace Ui {
class DialogCorrector;
}

class DialogCorrector : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogCorrector(Common *common, QWidget *parent = 0);
    ~DialogCorrector();
    
private slots:
    void on_lineEdit_returnPressed();

private:
    Ui::DialogCorrector *ui;
    Common *m_common;
};

#endif // DIALOGCORRECTOR_H
