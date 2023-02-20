#pragma once

#include "QtCore/qobject.h"
#include "author_dto_base.h"

#include <QDateTime>
#include <QObject>

namespace Contracts::DTO::Author
{
class CreateAuthorDTOData : public AuthorBaseData
{
  public:
    CreateAuthorDTOData()
    {
    }

    CreateAuthorDTOData *clone() const
    {
        return new CreateAuthorDTOData(*this);
    }
};

//-------------------------------------------------

class CreateAuthorDTO : public AuthorDTOBase
{
    Q_OBJECT
  public:
    CreateAuthorDTO(QObject *parent = nullptr) : AuthorDTOBase(parent)
    {
        m_data.reset(new CreateAuthorDTOData);
    }
    CreateAuthorDTO(const QString &name, const QUuid &relative) : AuthorDTOBase(nullptr)
    {
        m_data.reset(new CreateAuthorDTOData);
        m_data->name = name;
        m_data->relative = relative;
    }

    CreateAuthorDTO(const CreateAuthorDTO &other) : AuthorDTOBase(other), m_data(other.m_data->clone())
    {
    }

    CreateAuthorDTO &operator=(const CreateAuthorDTO &other)
    {
        if (this != &other)
        {
            m_data.reset(other.m_data->clone());
        }
        return *this;
    }

    CreateAuthorDTO clone()
    {
        return CreateAuthorDTO(*this);
    }

  private:
    QExplicitlySharedDataPointer<CreateAuthorDTOData> m_data;
};

} // namespace Contracts::DTO::Author
