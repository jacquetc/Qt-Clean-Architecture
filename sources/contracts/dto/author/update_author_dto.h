#ifndef UPDATE_AUTHOR_DTO_H
#define UPDATE_AUTHOR_DTO_H

#include "author_dto_base.h"
#include <QUuid>

namespace Contracts::DTO::Author
{

class UpdateAuthorDTOData : public AuthorBaseData
{
  public:
    UpdateAuthorDTOData() : uuid(QUuid::createUuid())
    {
    }

    QUuid uuid;

    UpdateAuthorDTOData *clone() const
    {
        return new UpdateAuthorDTOData(*this);
    }
};

//-------------------------------------------------

class UpdateAuthorDTO : public AuthorDTOBase
{
    Q_OBJECT
    Q_PROPERTY(QUuid uuid READ getUuid WRITE setUuid)
  public:
    UpdateAuthorDTO(QObject *parent = nullptr) : AuthorDTOBase(parent)
    {
        m_data.reset(new UpdateAuthorDTOData);
    }
    UpdateAuthorDTO(const QUuid &uuid, const QString &name, const QUuid &relative) : AuthorDTOBase(nullptr)
    {
        m_data.reset(new UpdateAuthorDTOData);
        m_data->uuid = uuid;
        m_data->name = name;
        m_data->relative = relative;
    }

    UpdateAuthorDTO(const UpdateAuthorDTO &other) : AuthorDTOBase(other), m_data(other.m_data->clone())
    {
    }

    UpdateAuthorDTO &operator=(const UpdateAuthorDTO &other)
    {
        if (this != &other)
        {
            m_data.reset(other.m_data->clone());
        }
        return *this;
    }

    UpdateAuthorDTO clone()
    {
        return UpdateAuthorDTO(*this);
    }

    QUuid getUuid() const;
    QUuid uuid() const;
    void setUuid(const QUuid &newUuid);

  private:
    QExplicitlySharedDataPointer<UpdateAuthorDTOData> m_data;
};

//-------------------------------------------------

inline QUuid UpdateAuthorDTO::getUuid() const
{
    return static_cast<UpdateAuthorDTOData *>(m_data.data())->uuid;
}

inline QUuid UpdateAuthorDTO::uuid() const
{
    return static_cast<UpdateAuthorDTOData *>(m_data.data())->uuid;
}

inline void UpdateAuthorDTO::setUuid(const QUuid &newUuid)
{
    static_cast<UpdateAuthorDTOData *>(m_data.data())->uuid = newUuid;
}

} // namespace Contracts::DTO::Author

#endif // UPDATE_AUTHOR_DTO_H
