#ifndef AUTHOR_DTO_BASE_H
#define AUTHOR_DTO_BASE_H

#include <QDateTime>
#include <QObject>
#include <QSharedData>
#include <QString>
#include <QUuid>

namespace Contracts::DTO::Author
{

class AuthorBaseData : public QSharedData
{
  public:
    AuthorBaseData() : creationDate(QDateTime::currentDateTime()), updateDate(QDateTime::currentDateTime())
    {
    }

    QDateTime creationDate;
    QDateTime updateDate;
    QString name;
    QUuid relative;
};

class AuthorDTOBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(QUuid relative READ getRelative WRITE setRelative)
    Q_PROPERTY(QDateTime creationDate READ getCreationDate WRITE setCreationDate)
    Q_PROPERTY(QDateTime updateDate READ getUpdateDate WRITE setUpdateDate)

  public:
    AuthorDTOBase(QObject *parent = nullptr) : QObject(parent)
    {
        m_data = new AuthorBaseData;
    }

    AuthorDTOBase(const AuthorDTOBase &other) : QObject(other.parent()), m_data(other.m_data)
    {
    }

    AuthorDTOBase &operator=(const AuthorDTOBase &other)
    {
        if (this != &other)
        {
            m_data.detach();
            m_data = other.m_data;
        }
        return *this;
    }

    AuthorDTOBase clone()
    {
        AuthorDTOBase dto(this);
        dto.m_data.detach();
        return dto;
    }

    QDateTime getCreationDate() const;
    QDateTime creationDate() const;
    void setCreationDate(const QDateTime &newCreationDate);
    QDateTime getUpdateDate() const;
    QDateTime updateDate() const;
    void setUpdateDate(const QDateTime &newUpdateDate);
    QString getName() const;
    QString name() const;
    void setName(const QString &newName);
    QUuid getRelative() const;
    QUuid relative() const;
    void setRelative(const QUuid &newRelative);

  private:
    QExplicitlySharedDataPointer<AuthorBaseData> m_data;
};

inline QDateTime AuthorDTOBase::getCreationDate() const
{
    return m_data->creationDate;
}

inline QDateTime AuthorDTOBase::creationDate() const
{
    return m_data->creationDate;
}

inline void AuthorDTOBase::setCreationDate(const QDateTime &newCreationDate)
{
    m_data->creationDate = newCreationDate;
}

inline QDateTime AuthorDTOBase::getUpdateDate() const
{
    return m_data->updateDate;
}

inline QDateTime AuthorDTOBase::updateDate() const
{
    return m_data->updateDate;
}

inline void AuthorDTOBase::setUpdateDate(const QDateTime &newUpdateDate)
{
    m_data->updateDate = newUpdateDate;
}

inline QUuid AuthorDTOBase::getRelative() const
{
    return m_data->relative;
}

inline QUuid AuthorDTOBase::relative() const
{
    return m_data->relative;
}

inline void AuthorDTOBase::setRelative(const QUuid &newRelative)
{
    m_data->relative = newRelative;
}

inline QString AuthorDTOBase::getName() const
{
    return m_data->name;
}

inline QString AuthorDTOBase::name() const
{
    return m_data->name;
}

inline void AuthorDTOBase::setName(const QString &newName)
{
    m_data->name = newName;
}

} // namespace Contracts::DTO::Author
#endif // AUTHOR_DTO_BASE_H
