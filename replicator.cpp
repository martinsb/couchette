#include "replicator.h"

#include <QObject>
#include <QUrl>
#include <QList>
#include <QPair>
#include <QString>

#include <QDebug>

namespace Couchette
{

Replicator::Replicator(LocalStorage *storage)
                : QObject(storage), m_lastSeq(0)
{
    QObject::connect(&m_monitor, SIGNAL(changes(const Changes*)),
                    this, SLOT(_onChanges(const Changes*)));

    QObject::connect(&m_monitor, SIGNAL(disconnected()),
                    this, SLOT(_onDisconnected()));

    m_storage = storage;
}

Replicator::~Replicator()
{

}

void Replicator::replicate(const QUrl& url)
{
    QUrl queryUrl(url);
    QString path = queryUrl.path();
    if (!path.endsWith('/')) {
        path.append('/');
    }
    path.append("_changes");
    queryUrl.setPath(path);

    QList<QPair<QString, QString> > params;
    params.append(QPair<QString, QString>("feed", "continuous"));
    params.append(QPair<QString, QString>("style", "all_docs"));
    params.append(QPair<QString, QString>("since", QString::number(static_cast<qulonglong>(m_storage->sequence()))));
    queryUrl.setQueryItems(params);

    m_monitor.start(queryUrl);
    m_url = queryUrl;
}

void Replicator::_onChanges(const Changes* changes)
{
    qDebug() << "Received changes:" << (changes->sequence());
    quint64 seq = changes->sequence();
    if (changes->deleted()) {
        m_storage->deleteDocument(changes->documentId());
    }
    else {
        QList<QPair<QString, QString> > revs;
        revs.append(QPair<QString, QString>(changes->documentId(), changes->revision()));
        QList<QPair<QString, QString> > newRevs = m_storage->revsDiff(revs);

    }
    m_storage->setSequence(seq);
}

void Replicator::_onDisconnected()
{
    qDebug() << "Disconnected";
    m_monitor.stop();
    m_monitor.start(m_url);
}

}

