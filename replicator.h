#ifndef _REPLICATOR_H
#define _REPLICATOR_H

#include "monitor.h"
#include "localstorage.h"

#include <QObject>
#include <QUrl>

namespace Couchette
{

class Replicator : public QObject
{
    Q_OBJECT

public:
    Replicator(LocalStorage* storage);
    virtual ~Replicator();

    void replicate(const QUrl& url);

private:
    Monitor m_monitor;
    QUrl m_url;
    qint64 m_lastSeq;

    LocalStorage *m_storage;

private slots:
   void _onChanges(const Changes* changes);
   void _onDisconnected(); 

};

}

#endif //_REPLICATOR_H
