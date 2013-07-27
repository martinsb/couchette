#ifndef _SQLITESTORAGE_H
#define _SQLITESTORAGE_H

#include "localstorage.h"

#include <QSqlDatabase>
#include <QSqlQuery>

namespace Couchette
{

class SqliteStorage : public LocalStorage
{
    Q_OBJECT

public:
    SqliteStorage(QObject *parent = NULL);
    virtual ~SqliteStorage();

public:
    virtual quint64 sequence();

    virtual void setSequence(quint64 seqId);

    virtual void storeDocument(const QVariantMap& document);

    virtual QVariantMap retrieveDocument(const QString& docId);

    virtual QList<QPair<QString, QString> > revsDiff(const QList<QPair<QString, QString> > input);

    virtual void deleteDocument(const QString& docId);

private:
    QSqlDatabase m_db;

};

}

#endif //_SQLITESTORAGE_H
