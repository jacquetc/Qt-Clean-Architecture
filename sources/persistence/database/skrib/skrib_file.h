#pragma once

#include "QtCore/qdebug.h"
#include "QtCore/qmetaobject.h"
#include "database/interface_database.h"
#include "database/interface_database_context.h"
#include "database/skrib/skrib_file_context.h"
#include "persistence_global.h"
#include "result.h"
#include <QMetaObject>
#include <QReadWriteLock>
#include <QSqlError>
#include <QSqlQuery>

using namespace Contracts::Database;

namespace Database::Skrib
{
template <class T> class SKRPERSISTENCEEXPORT SkribFile : public InterfaceDatabase<T>
{
  public:
    explicit SkribFile(InterfaceDatabaseContext *context);
    SkribFile(const SkribFile &other);

    Result<T> get(const QUuid &uuid) override;
    Result<QList<T>> getAll() override;
    Result<T> remove(T &&entity) override;
    Result<T> add(T &&entity) override;
    Result<T> update(T &&entity) override;
    Result<bool> exists(const QUuid &uuid) override;

    InterfaceDatabaseContext *databaseContext() const override;
    void setDatabaseContext(InterfaceDatabaseContext *newDatabaseContext) override;

  private:
    mutable QReadWriteLock m_databaseLock;
    QScopedPointer<InterfaceDatabaseContext> m_databaseContext;

    static QString getEntityClassName();
    static QStringList getEntityProperties();
    static Result<T> mapToEntity(const QHash<QString, QVariant> &fieldWithValue);
};

//--------------------------------------------

template <class T> SkribFile<T>::SkribFile(InterfaceDatabaseContext *context) : m_databaseContext(context)
{
}

//--------------------------------------------

template <class T> SkribFile<T>::SkribFile(const SkribFile &other) : m_databaseContext(other.databaseContext())
{
}

//--------------------------------------------

template <class T> Result<T> SkribFile<T>::get(const QUuid &uuid)
{
    const QString &entityName = SkribFile::getEntityClassName();
    const QStringList &properties = SkribFile::getEntityProperties();
    QSqlDatabase database =
        QSqlDatabase::database(dynamic_cast<SkribFileContext *>(this->databaseContext())->databaseName());
    QHash<QString, QVariant> fieldWithValue;

    QString fields;
    for (const QString &property : properties)
    {
        fields += property + ",";
    }
    fields.chop(1);

    {
        QReadLocker locker(&m_databaseLock); // Acquire a read lock before reading from the database
        QSqlQuery query(database);
        QString queryStr = "SELECT " + fields + " FROM " + entityName + " WHERE uuid = :uuid";
        query.prepare(queryStr);
        query.bindValue(":uuid", QVariant(uuid));
        query.exec();

        if (query.lastError().isValid())
        {
            return Result<T>(Error("SkribFile", Error::Critical, "sql_error", query.lastError().text(), queryStr));
        }

        while (query.next())
        {
            for (int i = 0; i < properties.count(); i++)
            {
                fieldWithValue.insert(properties.at(i), query.value(i));
            }
        }
        if (fieldWithValue.isEmpty())
        {
            return Result<T>(
                Error("SkribFile", Error::Critical, "sql_row_missing", "No row wiht uuid " + uuid.toString()));
        }
    }

    return SkribFile::mapToEntity(fieldWithValue);
}

//--------------------------------------------

template <class T> Result<QList<T>> SkribFile<T>::getAll()
{
    const QString &entityName = SkribFile::getEntityClassName();
    const QStringList &properties = SkribFile::getEntityProperties();
    QSqlDatabase database =
        QSqlDatabase::database(dynamic_cast<SkribFileContext *>(this->databaseContext())->databaseName());
    QList<QHash<QString, QVariant>> fieldsWithValues;
    QList<T> entities;

    QString fields;
    for (const QString &property : properties)
    {
        fields += property + ",";
    }
    fields.chop(1);

    {
        QReadLocker locker(&m_databaseLock); // Acquire a read lock before reading from the database
        QSqlQuery query(database);
        QString queryStr = "SELECT " + fields + " FROM " + entityName;
        query.prepare(queryStr);
        query.exec();

        if (query.lastError().isValid())
        {
            return Result<QList<T>>(
                Error("SkribFile", Error::Critical, "sql_error", query.lastError().text(), queryStr));
        }

        while (query.next())
        {
            QHash<QString, QVariant> fieldWithValue;
            for (int i = 0; i < properties.count(); i++)
            {
                fieldWithValue.insert(properties.at(i), query.value(i));
            }
            fieldsWithValues.append(fieldWithValue);
        }
    }

    for (const auto &fieldWithValue : fieldsWithValues)
    {
        Result<T> entity = SkribFile::mapToEntity(fieldWithValue);
        if (entity.hasError())
        {
            return Result<QList<T>>(entity.error());
        }
        entities.append(entity.value());
    }

    return Result<QList<T>>(entities);
}

//--------------------------------------------

template <class T> Result<T> SkribFile<T>::remove(T &&entity)
{
    const QString &entityName = SkribFile::getEntityClassName();
    QSqlDatabase database =
        QSqlDatabase::database(dynamic_cast<SkribFileContext *>(this->databaseContext())->databaseName());

    // Generate the SQL DELETE statement
    QString queryStr = "DELETE FROM " + entityName + " WHERE uuid = :uuid";

    {
        QWriteLocker locker(&m_databaseLock); // Acquire a write lock before writing to the database
        QSqlQuery query(database);
        query.prepare(queryStr);
        query.bindValue(":uuid", entity.uuid().toString());

        // Execute the DELETE statement with the entity UUID
        if (!query.exec())
        {
            return Result<T>(Error("SkribFile", Error::Critical, "sql_error", query.lastError().text(), queryStr));
        }

        // Return an appropriate Result object based on the query execution result
        if (query.numRowsAffected() == 1)
        {
            return Result<T>(std::forward<T>(entity));
        }
        else
        {
            return Result<T>(Error("SkribFile", Error::Critical, "sql_delete_failed",
                                   "Failed to delete row from database", entity.uuid().toString()));
        }
    }
}

//--------------------------------------------

template <class T> Result<T> SkribFile<T>::add(T &&entity)
{
    const QString &entityName = SkribFile::getEntityClassName();
    const QStringList &properties = SkribFile::getEntityProperties();
    QHash<QString, QVariant> fieldWithValue;

    // Get the entity properties and the corresponding values
    for (const QString &property : properties)
    {
        QVariant value = entity.property(property.toLatin1());
        fieldWithValue.insert(property, value);
    }

    // Generate the SQL INSERT statement with placeholders for the property values
    QString fields;
    QString placeholders;
    for (const QString &property : properties)
    {
        fields += property + ",";
        placeholders += ":" + property + ",";
    }
    fields.chop(1);
    placeholders.chop(1);
    QString queryStr = "INSERT INTO " + entityName + " (" + fields + ") VALUES (" + placeholders + ")";

    QSqlDatabase database =
        QSqlDatabase::database(dynamic_cast<SkribFileContext *>(this->databaseContext())->databaseName());

    {
        QWriteLocker locker(&m_databaseLock); // Acquire a write lock before writing to the database
        QSqlQuery query(database);
        query.prepare(queryStr);

        // Bind the property values to the placeholders in the SQL statement
        for (const QString &property : properties)
        {
            QVariant value = fieldWithValue.value(property);
            query.bindValue(":" + property, value);
        }

        // Execute the INSERT statement with the entity property values
        if (!query.exec())
        {
            return Result<T>(Error("SkribFile", Error::Critical, "sql_error", query.lastError().text(), queryStr));
        }

        // Return an appropriate Result object based on the query execution result
        if (query.numRowsAffected() == 1)
        {
            return Result<T>(std::forward<T>(entity));
        }
        else
        {
            return Result<T>(
                Error("SkribFile", Error::Critical, "sql_insert_failed", "Failed to insert row into database"));
        }
    }
}

//--------------------------------------------

template <class T> Result<T> SkribFile<T>::update(T &&entity)
{
    const QString &entityName = SkribFile::getEntityClassName();
    const QStringList &properties = SkribFile::getEntityProperties();
    QHash<QString, QVariant> fieldWithValue;

    // Get the entity properties and the corresponding values
    for (const QString &property : properties)
    {
        QVariant value = entity.property(property.toLatin1());
        fieldWithValue.insert(property, value);
    }

    // Generate the SQL UPDATE statement with placeholders for the property values
    QString fields;
    for (const QString &property : properties)
    {
        fields += property + " = :" + property + ",";
    }
    fields.chop(1);
    QString queryStr = "UPDATE " + entityName + " SET " + fields + " WHERE uuid = :uuid";

    QSqlDatabase database =
        QSqlDatabase::database(dynamic_cast<SkribFileContext *>(this->databaseContext())->databaseName());

    {
        QWriteLocker locker(&m_databaseLock); // Acquire a write lock before writing to the database
        QSqlQuery query(database);
        query.prepare(queryStr);

        // Bind the property values to the placeholders in the SQL statement
        for (const QString &property : properties)
        {
            QVariant value = fieldWithValue.value(property);
            query.bindValue(":" + property, value);
        }

        // Bind the UUID value to the placeholder in the SQL statement
        query.bindValue(":uuid", fieldWithValue.value("uuid"));

        // Execute the UPDATE statement with the entity property values
        if (!query.exec())
        {
            return Result<T>(Error("SkribFile", Error::Critical, "sql_error", query.lastError().text(), queryStr));
        }

        // Return an appropriate Result object based on the query execution result
        if (query.numRowsAffected() == 1)
        {
            return Result<T>(std::forward<T>(entity));
        }
        else
        {
            return Result<T>(
                Error("SkribFile", Error::Critical, "sql_update_failed", "Failed to update row in database"));
        }
    }
}

//--------------------------------------------

template <class T> Result<bool> SkribFile<T>::exists(const QUuid &uuid)
{
    const QString &entityName = SkribFile::getEntityClassName();
    QSqlDatabase database =
        QSqlDatabase::database(dynamic_cast<SkribFileContext *>(this->databaseContext())->databaseName());

    {
        QReadLocker locker(&m_databaseLock); // Acquire a read lock before reading from the database
        QSqlQuery query(database);
        QString queryStr = "SELECT COUNT(*) FROM " + entityName + " WHERE uuid = :uuid";
        query.prepare(queryStr);
        query.bindValue(":uuid", uuid.toString());
        query.exec();

        if (query.lastError().isValid())
        {
            return Result<bool>(Error("SkribFile", Error::Critical, "sql_error", query.lastError().text(), queryStr));
        }

        if (query.next())
        {
            return Result<bool>(query.value(0).toBool());
        }
        else
        {
            return Result<bool>(
                Error("SkribFile", Error::Critical, "sql_row_missing", "No row with uuid " + uuid.toString()));
        }
    }
}

//--------------------------------------------

template <class T> InterfaceDatabaseContext *SkribFile<T>::databaseContext() const
{
    return m_databaseContext.get();
}

//--------------------------------------------

template <class T> void SkribFile<T>::setDatabaseContext(InterfaceDatabaseContext *newDatabaseContext)
{
    m_databaseContext.reset(newDatabaseContext);
}

//--------------------------------------------

template <class T> QString SkribFile<T>::getEntityClassName()
{

    const QMetaObject &sourceMetaObject = T::staticMetaObject;

    return QString(sourceMetaObject.className()).split("::").last();
}

//--------------------------------------------

template <class T> QStringList SkribFile<T>::getEntityProperties()
{
    QStringList propertyList;

    const QMetaObject &metaObject = T::staticMetaObject;
    int propertyCount = metaObject.propertyCount();

    for (int i = 0; i < propertyCount; ++i)
    {
        QMetaProperty property = metaObject.property(i);
        if (property.isReadable())
        {
            if (property.name() == QString("objectName"))
            {
                continue;
            }
            propertyList.append(property.name());
        }
    }

    return propertyList;
}

//--------------------------------------------

template <class T> Result<T> SkribFile<T>::mapToEntity(const QHash<QString, QVariant> &fieldWithValue)
{
    T entity;
    const QMetaObject *metaObject = entity.metaObject();

    QHash<QString, QVariant>::const_iterator i = fieldWithValue.constBegin();
    while (i != fieldWithValue.constEnd())
    {

        QString propertyName = i.key();

        int destinationPropertyIndex = metaObject->indexOfProperty(propertyName.toLatin1());
        if (destinationPropertyIndex >= 0)
        {
            QVariant value = i.value();
            QMetaProperty destinationProperty = metaObject->property(destinationPropertyIndex);

            if (destinationProperty.isWritable() &&
                QMetaType::canConvert(value.metaType(), destinationProperty.metaType()))
            {
                bool success = destinationProperty.write(&entity, value);
                if (!success)
                {
                    Result<T>(Error("SkribFile", Error::Fatal, "map_write_failed",
                                    "Failed to write value to destination property", propertyName));
                }
            }
        }
        else
        {
            Result<T>(Error("SkribFile", Error::Fatal, "map_missing_property", "Missing property in destination object",
                            propertyName));
        }
        ++i;
    }
    return Result<T>(entity);
}

} // namespace Database::Skrib
