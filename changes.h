#ifndef _CHANGES_H
#define _CHANGES_H

#include <QString>

namespace Couchette
{

class Changes
{
  
public:
    Changes();
    Changes(const Changes& other);
    virtual ~Changes();

    bool initWithJson(const QString& serialized);


    quint64 sequence() const;
    QString documentId() const;
    bool deleted() const;
    QString revision() const;

private:

private:
    quint64 m_seq;
    QString m_docId;
    QString m_rev;
    bool m_deleted;
};

}

#endif //_CHANGES_H
