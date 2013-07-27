#include "monitor.h"
#include "changes.h"

#include <QNetworkRequest>

#include <QDebug>

namespace Couchette
{

Monitor::Monitor(QObject *parent)
            : QObject(parent), m_currentReply(NULL), m_replyStream(NULL)
{

}

Monitor::~Monitor()
{
    stop();
}

void Monitor::start(const QUrl& url)
{
    _destroyCurrentStream();
    m_currentReply = m_networkManager.get(QNetworkRequest(url));
    if (m_currentReply) {
        m_replyStream = new QTextStream(m_currentReply);
        QObject::connect(m_currentReply, SIGNAL(downloadProgress(qint64, qint64)),
                    this, SLOT(_onDownloadProgress(qint64, qint64)));
        QObject::connect(m_currentReply, SIGNAL(finished()),
                    this, SLOT(_onRequestFinished()));
    }
}

void Monitor::stop()
{
    _destroyCurrentStream();
    if (m_currentReply) {
        m_currentReply->abort();
    }
}

void Monitor::_onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    QString json = m_replyStream->readLine();
    while (!json.isEmpty()) {
        qDebug() << json;
        Changes c;
        if (c.initWithJson(json)) {
            emit this->changes(&c);
        }
        json = m_replyStream->readLine();
    }
}

void Monitor::_onRequestFinished()
{
    _destroyCurrentStream();
    if (m_currentReply) {
        m_currentReply->deleteLater();
    }
    emit disconnected();
}

void Monitor::_destroyCurrentStream()
{
    if (m_replyStream) {
        delete m_replyStream;
        m_replyStream = NULL;
    }
}

}
