#include "systemtray.h"
#include <QDebug>
#include <QLineEdit>
#include <QMessageBox>
#include <QIcon>

SystemTray::SystemTray(Common *common, QObject *parent) :
    QSystemTrayIcon(parent),
    m_common(common)
{
    this->setIcon(QIcon(":/images/icon.png"));
    this->show();

    m_contextMenu = new QMenu();
    connect(m_contextMenu->addAction(QIcon(":/images/system-search.svg"), tr("Quick details")), SIGNAL(triggered()), this, SLOT(quickDetails()));
    m_contextMenu->addSeparator();
    connect(m_contextMenu->addAction(QIcon(":/images/system-log-out.svg"), tr("Exit")), SIGNAL(triggered()), this, SLOT(exitAction()));

    this->setContextMenu(m_contextMenu);
    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(activatedSlot(QSystemTrayIcon::ActivationReason)));
    itemDetails = new DialogItemDetails(m_common);
}

SystemTray::~SystemTray()
{
    delete itemDetails;
}

void SystemTray::fileImported(QString path, double price)
{
    this->showMessage(tr("File imported"), tr("The %1 file had been imported\nTotal %2 Ft goods imported").arg(path).arg(price), QSystemTrayIcon::Information, 100000);
}

void SystemTray::fileImportFailed(QString path)
{
    this->showMessage(tr("File import failed"), tr("The %1 file had not been imported").arg(path), QSystemTrayIcon::Critical);
}

void SystemTray::exitAction()
{
    m_common->askForExit();
}

void SystemTray::quickDetails()
{
    itemDetails->show();
}

void SystemTray::activatedSlot(QSystemTrayIcon::ActivationReason reason)
{
    if (reason != QSystemTrayIcon::Context) {
        emit mainWindowRequested();
    }
}
