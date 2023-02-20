#pragma once

#include <QDateTime>
#include <QSharedData>
#include <QUuid>

namespace Domain
{
class EntityData : public QSharedData
{
  public:
    EntityData()
        : uuid(QUuid::createUuid()), creationDate(QDateTime::currentDateTime()),
          updateDate(QDateTime::currentDateTime())
    {
    }

    EntityData(const QUuid &uuid)
        : uuid(uuid), creationDate(QDateTime::currentDateTime()), updateDate(QDateTime::currentDateTime())
    {
    }

    EntityData(const QUuid &uuid, const QDateTime &creationDate, const QDateTime &updateDate)
        : uuid(uuid), creationDate(creationDate), updateDate(updateDate)
    {
    }
    EntityData *clone() const
    {
        return new EntityData(*this);
    }

    QUuid uuid;
    QDateTime creationDate;
    QDateTime updateDate;
};

class Entity : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUuid uuid READ getUuid WRITE setUuid)
    Q_PROPERTY(QDateTime creationDate READ getCreationDate WRITE setCreationDate)
    Q_PROPERTY(QDateTime updateDate READ getUpdateDate WRITE setUpdateDate)
  public:
    Entity() : QObject(), m_data(new EntityData)
    {
    }

    Entity(const QUuid &uuid) : QObject(), m_data(new EntityData(uuid))
    {
    }
    Entity(const QUuid &uuid, const QDateTime &creationDate, const QDateTime &updateDate)
        : QObject(), m_data(new EntityData(uuid, creationDate, updateDate))
    {
    }

    Entity(const Entity &entity) : QObject(), m_data(entity.m_data->clone())
    {
    }

    Entity &operator=(const Entity &other)
    {
        if (this != &other)
        {
            m_data.reset(other.m_data->clone());
        }
        return *this;
    }

    Entity clone() const
    {
        return Entity(*this);
    }

    QUuid getUuid() const
    {
        return m_data->uuid;
    }

    QUuid uuid() const
    {
        return m_data->uuid;
    }

    void setUuid(const QUuid &uuid)
    {
        m_data->uuid = uuid;
    }

    QDateTime getCreationDate() const
    {
        return m_data->creationDate;
    }

    QDateTime creationDate() const
    {
        return m_data->creationDate;
    }

    void setCreationDate(const QDateTime &creationDate)
    {
        m_data->creationDate = creationDate;
    }

    QDateTime getUpdateDate() const
    {
        return m_data->updateDate;
    }

    QDateTime updateDate() const
    {
        return m_data->updateDate;
    }

    void setUpdateDate(const QDateTime &updateDate)
    {
        m_data->updateDate = updateDate;
    }

  private:
    QExplicitlySharedDataPointer<EntityData> m_data;
};
} // namespace Domain
