#ifndef DIALOGLOGIN_H
#define DIALOGLOGIN_H

#include <QDialog>
#include <QSettings>

#include "common.h"

namespace Ui {
class DialogLogin;
}

class DialogLogin : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogLogin(Common *common, QWidget *parent = 0);
    bool autoLogin();
    ~DialogLogin();
signals:
    void loginSuccessful();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogLogin *ui;
    Common *m_common;
    QSettings m_settings;

    void showLoginError();
};

#endif // DIALOGLOGIN_H
