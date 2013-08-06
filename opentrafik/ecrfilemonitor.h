#ifndef ECRFILEMONITOR_H
#define ECRFILEMONITOR_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QSettings>

class ECRFileMonitor : public QObject
{
    Q_OBJECT
public:
    explicit ECRFileMonitor(QObject *parent = 0);

    bool setPath(QString path = "");
    QString path() const {return m_path;}

private:
    QString m_path;
    QFileSystemWatcher *m_watcher;
    QSettings m_settings;
signals:
    void newFileImported(QString path);
public slots:
    
private slots:
    void fileChangedSlot(const QString &path);
};

#endif // ECRFILEMONITOR_H
