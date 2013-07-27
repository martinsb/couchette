#ifndef _MONITOR_H
#define _MONITOR_H

#include "changes.h"

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextStream>
#include <QString>

namespace Couchette
{

class Monitor : public QObject
{
    Q_OBJECT

public:
    Monitor(QObject *parent = NULL);
    virtual ~Monitor();

    void start(const QUrl& url);

    void stop();

signals:
    void changes(const Changes* changes);
    void disconnected();

private:
    QNetworkAccessManager m_networkManager;
    QNetworkReply *m_currentReply;
    QTextStream *m_replyStream;

private slots:
    void _onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void _onRequestFinished();

    void _destroyCurrentStream();
};

}

#endif //_MONITOR_H
