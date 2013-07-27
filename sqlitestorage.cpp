#include "sqlitestorage.h"

#include <QSqlDatabase>
#include <QSqlQuery>

#include <QDebug>

namespace Couchette
{

SqliteStorage::SqliteStorage(QObject *parent)
                        : LocalStorage(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("testdb.sqlite");
    if (m_db.open()) {
        QSqlQuery query(m_db);
        if (query.exec("CREATE TABLE IF NOT EXISTS couch_variables (last_seq bigint);")) {
            QSqlQuery insert(m_db);
            insert.exec("SELECT COUNT(last_seq) FROM couch_variables");
            if (insert.next() && insert.value(0).toInt() == 0) {
                QSqlQuery(m_db).exec("INSERT INTO couch_variables (last_seq) VALUES (0)");
            }
        }
        else {
            qDebug() << "Could not execute couch_variables table creation query";
        }

        QSqlQuery documentsQuery(m_db);
        if (!documentsQuery.exec("CREATE TABLE IF NOT EXISTS couch_documents (doc_id varchar(255), doc_revision varchar(255));")) {
            qDebug() << "Could not execute couch_documents table creation query";
        }
    }
    else {
        qDebug() << "Could not open database";
    }
}

SqliteStorage::~SqliteStorage()
{
    m_db.close();
}

quint64 SqliteStorage::sequence()
{
    QSqlQuery query(m_db);
    if (!query.exec("SELECT last_seq FROM couch_variables")) {
        qDebug() << "Could not execute sequence retrieval query";
    }
    if (query.next()) {
        return static_cast<quint64>(query.value(0).toULongLong());
    }
    return 0;
}

void SqliteStorage::setSequence(quint64 seqId)
{
    QSqlQuery update(m_db);
    update.prepare("UPDATE couch_variables SET last_seq=:seq");
    update.bindValue("seq", seqId);
    if (!update.exec()) {
        qDebug() << "Could not execute update couch_variables query";
    }
}


void SqliteStorage::storeDocument(const QVariantMap& document)
{
    
}

QVariantMap SqliteStorage::retrieveDocument(const QString& docId)
{
    return QVariantMap();
}

void SqliteStorage::deleteDocument(const QString& docId)
{
    QSqlQuery deleteQuery(m_db);
    deleteQuery.prepare("DELETE from couch_documents WHERE doc_id=:id");
    deleteQuery.bindValue("id", docId);
    if (!deleteQuery.exec()) {
        qDebug() << "Could not execute document deletion query";
    }
}

QList<QPair<QString, QString> > SqliteStorage::revsDiff(const QList<QPair<QString, QString> > input) 
{
    return input;
}


}
