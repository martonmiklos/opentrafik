#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QLineEdit>

#include "common.h"
#include "dialogitemdetails.h"

class SystemTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    SystemTray(Common *common, QObject *parent = 0);
    ~SystemTray();

private:
    QMenu *m_contextMenu;
    Common *m_common;
    DialogItemDetails *itemDetails;

signals:
    void mainWindowRequested();

public slots:
    void fileImported(QString path, double price);
    void fileImportFailed(QString path);

private slots:
    void exitAction();
    void quickDetails();
    void activatedSlot(QSystemTrayIcon::ActivationReason reason);
};

#endif // SYSTEMTRAY_H
