#ifndef AUTHOR_DTO_H
#define AUTHOR_DTO_H

#include "author_dto_base.h"
#include <QUuid>

namespace Contracts::DTO::Author
{

class AuthorDTOData : public AuthorBaseData
{
  public:
    AuthorDTOData() : uuid(QUuid::createUuid())
    {
    }

    QUuid uuid;

    AuthorDTOData *clone() const
    {
        return new AuthorDTOData(*this);
    }
};

//-------------------------------------------------

class AuthorDTO : public AuthorDTOBase
{
    Q_OBJECT
    Q_PROPERTY(QUuid uuid READ getUuid WRITE setUuid)
  public:
    AuthorDTO(QObject *parent = nullptr) : AuthorDTOBase(parent)
    {
        m_data.reset(new AuthorDTOData);
    }
    AuthorDTO(const QUuid &uuid, const QString &name, const QUuid &relative) : AuthorDTOBase(nullptr)
    {
        m_data.reset(new AuthorDTOData);
        m_data->uuid = uuid;
        m_data->name = name;
        m_data->relative = relative;
    }

    AuthorDTO(const AuthorDTO &other) : AuthorDTOBase(other), m_data(other.m_data->clone())
    {
    }

    AuthorDTO &operator=(const AuthorDTO &other)
    {
        if (this != &other)
        {
            m_data.reset(other.m_data->clone());
        }
        return *this;
    }

    AuthorDTO clone()
    {
        return AuthorDTO(*this);
    }

    QUuid getUuid() const;
    QUuid uuid() const;
    void setUuid(const QUuid &newUuid);

  private:
    QExplicitlySharedDataPointer<AuthorDTOData> m_data;
};

//-------------------------------------------------

inline QUuid AuthorDTO::getUuid() const
{
    return static_cast<AuthorDTOData *>(m_data.data())->uuid;
}
inline QUuid AuthorDTO::uuid() const
{
    return static_cast<AuthorDTOData *>(m_data.data())->uuid;
}

inline void AuthorDTO::setUuid(const QUuid &newUuid)
{
    static_cast<AuthorDTOData *>(m_data.data())->uuid = newUuid;
}

} // namespace Contracts::DTO::Author

#endif // AUTHOR_DTO_H
