#include "ecrfilemonitor.h"

#include <QDir>
#include <QDebug>
#include <QFile>
#include <QMessageBox>

ECRFileMonitor::ECRFileMonitor(QObject *parent) :
    QObject(parent)
{
    m_watcher = new QFileSystemWatcher(this);

    m_settings.beginGroup("ECR");
    QString path = m_settings.value("plu_reports_file").toString();
    m_settings.endGroup();

    setPath(path);
    connect(m_watcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChangedSlot(QString)));
}

bool ECRFileMonitor::setPath(QString path)
{
    if (path != "")
        m_path = path;

    QFileInfo watchDir(m_path);

    if (watchDir.isFile() && watchDir.exists()) {
        if (!m_watcher->directories().isEmpty())
            m_watcher->removePaths(m_watcher->directories());
        m_watcher->addPath(m_path);
        return true;
    }
    return false;
}

void ECRFileMonitor::fileChangedSlot(const QString &path)
{
    QFile file(path);
    QString last = "";
    if (file.open(QFile::ReadOnly)) {
        while (!file.atEnd()) {
            QString lineData = file.readLine();
            QStringList lineParts = lineData.split("\t");
            if (lineParts.size() >= 3) {
                last = lineParts.at(2);
            }
        }
        file.close();
    }

    if (QFile::exists(last)) {
        QMessageBox msgBox;
        msgBox.setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
        if (msgBox.question(0,
                                  tr("New PLU data to import available"),
                                  tr("A new PLU import had been done to the %1 file\nShould I import it?").arg(last),
                                  QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
            emit newFileImported(last);
    }
}
