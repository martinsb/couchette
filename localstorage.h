#ifndef _LOCALSTORAGE_H
#define _LOCALSTORAGE_H

#include <QVariant>
#include <QString>
#include <QObject>
#include <QList>
#include <QPair>

namespace Couchette
{

class LocalStorage : public QObject
{
    Q_OBJECT
    
public:
    LocalStorage(QObject *parent);
    virtual ~LocalStorage();

public:
    virtual quint64 sequence() = 0;

    virtual void setSequence(quint64 seqId) = 0;

    virtual void storeDocument(const QVariantMap& document) = 0;

    virtual QVariantMap retrieveDocument(const QString& docId) = 0;

    virtual void deleteDocument(const QString& docId) = 0;

    virtual QList<QPair<QString, QString> > revsDiff(const QList<QPair<QString, QString> > input) = 0;

};


}

#endif //_LOCALSTORAGE_H

