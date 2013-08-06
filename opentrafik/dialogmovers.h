#ifndef DIALOGMOVERS_H
#define DIALOGMOVERS_H

#include "dialogbase.h"

namespace Ui {
class DialogMovers;
}

class DialogMovers : public DialogBaseWithModel
{
    Q_OBJECT
    
public:
    DialogMovers(Common *common, QWidget *parent = 0);
    ~DialogMovers();

    void databaseConnected();
    
private slots:
    void on_comboBox_activated(int index);

private:
    Ui::DialogMovers *ui;
    QString m_sort;
};

#endif // DIALOGMOVERS_H
