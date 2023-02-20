#pragma once

#include "domain_global.h"
#include "entity.h"
#include <QSharedData>

namespace Domain
{

class AuthorData : public EntityData
{
  public:
    AuthorData() : relative(QUuid())
    {
    }

    QString name;
    QUuid relative;

    AuthorData *clone() const
    {
        return new AuthorData(*this);
    }
};

class SKRDOMAINEXPORT Author : public Entity
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(QUuid relative READ getRelative WRITE setRelative)

  public:
    Author() : Entity()
    {

        m_data.reset(new AuthorData);
    };

    Author(const QUuid &uuid, const QString &name, const QUuid &relative) : Entity(uuid)
    {
        m_data.reset(new AuthorData);
        m_data->name = name;
        m_data->relative = relative;
    }

    Author(const QUuid &uuid, const QString &name, const QUuid &relative, const QDateTime &creationDate,
           const QDateTime &updateDate)
        : Entity(uuid, creationDate, updateDate)
    {
        m_data.reset(new AuthorData);
        m_data->name = name;
        m_data->relative = relative;
    }

    Author(const Author &other) : Entity(other), m_data(other.m_data->clone())
    {
    }

    Author &operator=(const Author &other)
    {
        if (this != &other)
        {
            m_data.reset(other.m_data->clone());
            Entity::operator=(other);
        }
        return *this;
    }

    Author clone() const
    {
        return Author(*this);
    }

    QString getName() const
    {
        return m_data->name;
    }

    QString name() const
    {
        return m_data->name;
    }

    void setName(const QString &name)
    {
        m_data->name = name;
    }

    QUuid getRelative() const
    {
        return m_data->relative;
    }

    QUuid relative() const
    {
        return m_data->relative;
    }

    void setRelative(const QUuid &relative)
    {
        m_data->relative = relative;
    }

  private:
    QExplicitlySharedDataPointer<AuthorData> m_data;
};

} // namespace Domain
