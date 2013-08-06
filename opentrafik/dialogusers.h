#ifndef DIALOGUSERS_H
#define DIALOGUSERS_H

#include <QDialog>
#include "dialogadduser.h"
#include "common.h"

namespace Ui {
class DialogUsers;
}

class DialogUsers : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogUsers(Common *common, QWidget *parent = 0);
    ~DialogUsers();
    
private slots:
    void on_pushButtonAdd_clicked();

private:
    Ui::DialogUsers *ui;
    DialogAddUser *addUserDialog;
    Common *m_common;
};

#endif // DIALOGUSERS_H
