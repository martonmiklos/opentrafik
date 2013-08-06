#ifndef DIALOGADDUSER_H
#define DIALOGADDUSER_H

#include <QDialog>
#include "common.h"

namespace Ui {
class DialogAddUser;
}

class DialogAddUser : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogAddUser(Common *common, QWidget *parent = 0);
    ~DialogAddUser();
    
private slots:
    void on_pushButtonAdd_clicked();

private:
    Ui::DialogAddUser *ui;
    void inputDataError(QString error);
    Common *m_common;
};

#endif // DIALOGADDUSER_H
