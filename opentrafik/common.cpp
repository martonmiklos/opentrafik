#include "common.h"
#include <QCryptographicHash>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QCoreApplication>

Common::Common(QObject *parent) :
    QObject(parent)
{
    m_sqlHelper = new SqlHelper(this);
    m_ecrDuFileReader = new ECRduFileReader(this);
}

Common::~Common()
{
    delete m_ecrDuFileReader;
}

QString Common::hashedPassword(const QString password)
{
    return QCryptographicHash::hash(QString("%1").arg(password+salt()).toLatin1(), QCryptographicHash::Sha1).toHex();
}


bool Common::login(QString userName, QString password, bool hashed)
{
    return m_sqlHelper->login(userName, password, hashed, &m_currentUserData);
}


void Common::askForExit()
{
    QMessageBox questionBox;
    questionBox.setWindowTitle(tr("Do you really want to quit?"));
    questionBox.setText(tr("Are you sure that you want to exit?"));
    questionBox.addButton(QMessageBox::Yes);
    questionBox.addButton(QMessageBox::No);
    questionBox.setDefaultButton(QMessageBox::Yes);
    questionBox.setStyleSheet("font: 26pt \"Ubuntu\";\n QPushButton{  background-color: rgb(163, 182, 255); }");
    if (questionBox.exec() == QMessageBox::Yes) {
        QSqlDatabase db = QSqlDatabase::database();
        if (db.isOpen())
            db.close();
        QCoreApplication::exit(0);
    }
}
