#include "changes.h"
#include "json.h"

#include <QVariant>
#include <QMap>
#include <QList>

namespace Couchette
{

Changes::Changes()
    : m_seq(0), m_deleted(false)
{

}

Changes::Changes(const Changes& other)
{
    m_seq = other.m_seq;
    m_docId = other.m_docId;
    m_rev = other.m_rev;
    m_deleted = other.m_deleted;
}

Changes::~Changes()
{

}

bool Changes::initWithJson(const QString& serialized)
{
    bool ok;
    QVariant object = QtJson::parse(serialized, ok);
    if (!ok)
        return false;
    if (object.type() != QVariant::Map)
        return false;

    QMap<QString, QVariant> map = object.toMap();
    if (!map.contains("seq") || !map.contains("id") || !map.contains("changes"))
        return false;

    QList<QVariant> list = map["changes"].toList();
    if (!list.length())
        return false;
    QMap<QString, QVariant> revMap = list[0].toMap();
    if (!revMap.contains("rev"))
        return false;

    m_seq = static_cast<quint64>(map["seq"].toULongLong());
    m_docId = map["id"].toString();
    m_rev = revMap["rev"].toString();
    m_deleted = map.contains("deleted") && map["deleted"].toBool();
    
    return true;
}

quint64 Changes::sequence() const
{
    return m_seq;
}

QString Changes::documentId() const
{
    return m_docId;
}

bool Changes::deleted() const
{
    return m_deleted;
}

QString Changes::revision() const
{
    return m_rev;
}

}
