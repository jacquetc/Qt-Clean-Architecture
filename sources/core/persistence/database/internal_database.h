#pragma once

#include "QtConcurrent/qtconcurrenttask.h"
#include "QtCore/qdebug.h"
#include "QtCore/qmetaobject.h"
#include "database/interface_database.h"
#include "database/interface_database_context.h"
#include "persistence_global.h"
#include "result.h"
#include <QDateTime>
#include <QMetaObject>
#include <QReadWriteLock>
#include <QSqlError>
#include <QSqlQuery>
#include <QTime>

using namespace Contracts::Database;

namespace Database
{ /**
   * @brief Template implementation of an InterfaceDatabase for a SQLite file database
   *
   * @tparam T Type of entity that the database will store
   */
template <class T> class SKRPERSISTENCEEXPORT InternalDatabase : public InterfaceDatabase<T>
{
  public:
    /**
     * @brief Construct a new Skrib File object
     *
     * @param context The context of the database
     */
    explicit InternalDatabase(InterfaceDatabaseContext *context);

    /**
     * @brief Copy constructor of Skrib File object
     *
     * @param other The object to be copied
     */
    InternalDatabase(const InternalDatabase &other);

    /**
     * @brief get retrieves an entity from the database with the specified UUID.
     * @param uuid The UUID of the entity to retrieve.
     * @return A Result object containing either the requested entity or an error message.
     */
    Result<T> get(const QUuid &uuid) override;

    /**
     * @brief getAll retrieves all entities of the type associated with this database.
     * @return A Result object containing either a list of entities or an error message.
     */
    Result<QList<T>> getAll() override;

    /**
     * @brief remove deletes an entity from the database.
     * @param entity The entity to delete.
     * @return A Result object containing either the deleted entity or an error message.
     */
    Result<T> remove(T &&entity) override;

    /**
     * @brief add adds an entity to the database.
     * @param entity The entity to add.
     * @return A Result object containing either the added entity or an error message.
     */
    Result<T> add(T &&entity) override;

    /**
     * @brief update updates an entity in the database.
     * @param entity The entity to update.
     * @return A Result object containing either the updated entity or an error message.
     */
    Result<T> update(T &&entity) override;

    /**
     * @brief exists checks whether an entity with the specified UUID exists in the database.
     * @param uuid The UUID of the entity to check for.
     * @return A Result object containing either a boolean indicating whether the entity exists
     * or an error message.
     */
    Result<bool> exists(const QUuid &uuid) override;

  private:
    QScopedPointer<InterfaceDatabaseContext>
        m_databaseContext; /**< A QScopedPointer that holds the InterfaceDatabaseContext associated with this SkribFile.
                            */

    /**
     * @brief getEntityClassName returns the name of the class associated with this database.
     * @return The class name as a QString.
     */
    static QString getEntityClassName();

    /**
     * @brief getEntityProperties returns the list of properties associated with the class
     * associated with this database.
     * @return A QStringList containing the property names.
     */
    static QStringList getEntityProperties();
    /**
     * @brief Maps a hash of field names to their corresponding values to an entity of type T.
     * @param fieldWithValue The hash of field names to their corresponding values to be mapped.
     * @return Result<T> The result of the mapping operation, containing either the mapped entity of type T or an error.
     */
    static Result<T> mapToEntity(const QHash<QString, QVariant> &fieldWithValue);
};

//--------------------------------------------

template <class T> InternalDatabase<T>::InternalDatabase(InterfaceDatabaseContext *context) : m_databaseContext(context)
{
}

//--------------------------------------------

template <class T>
InternalDatabase<T>::InternalDatabase(const InternalDatabase &other) : m_databaseContext(other.databaseContext())
{
}

//--------------------------------------------

template <class T> Result<T> InternalDatabase<T>::get(const QUuid &uuid)
{
    return QtConcurrent::task([this](QUuid uuid) {
               const QString &entityName = InternalDatabase::getEntityClassName();
               const QStringList &properties = InternalDatabase::getEntityProperties();
               QSqlDatabase database = QSqlDatabase::database(m_databaseContext->databaseName());
               QHash<QString, QVariant> fieldWithValue;

               QString fields;
               for (const QString &property : properties)
               {
                   fields += property + ",";
               }
               fields.chop(1);

               {
                   QSqlQuery query(database);
                   QString queryStr = "SELECT " + fields + " FROM " + entityName + " WHERE uuid = :uuid";
                   query.prepare(queryStr);
                   query.bindValue(":uuid", QVariant(uuid));
                   query.exec();

                   if (query.lastError().isValid())
                   {
                       return Result<T>(
                           Error("SkribFile", Error::Critical, "sql_error", query.lastError().text(), queryStr));
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
                       return Result<T>(Error("SkribFile", Error::Critical, "sql_row_missing",
                                              "No row wiht uuid " + uuid.toString()));
                   }
               }

               return InternalDatabase::mapToEntity(fieldWithValue);
           })
        .withArguments(uuid)
        .onThreadPool(m_databaseContext->threadPool())
        .spawn()
        .result();
}

//--------------------------------------------

template <class T> Result<QList<T>> InternalDatabase<T>::getAll()
{

    return QtConcurrent::task([this]() {
               const QString &entityName = InternalDatabase::getEntityClassName();
               const QStringList &properties = InternalDatabase::getEntityProperties();
               QSqlDatabase database = QSqlDatabase::database(m_databaseContext->databaseName());
               QList<QHash<QString, QVariant>> fieldsWithValues;
               QList<T> entities;

               QString fields;
               for (const QString &property : properties)
               {
                   fields += property + ",";
               }
               fields.chop(1);

               {
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
                   Result<T> entity = InternalDatabase::mapToEntity(fieldWithValue);
                   if (entity.hasError())
                   {
                       return Result<QList<T>>(entity.error());
                   }
                   entities.append(entity.value());
               }

               return Result<QList<T>>(entities);
           })

        .onThreadPool(m_databaseContext->threadPool())
        .spawn()
        .result();
}

//--------------------------------------------

template <class T> Result<T> InternalDatabase<T>::remove(T &&entity)
{
    return QtConcurrent::task([this](T entity) {
               const QString &entityName = InternalDatabase::getEntityClassName();
               QSqlDatabase database = QSqlDatabase::database(m_databaseContext->databaseName());

               // Generate the SQL DELETE statement
               QString queryStr = "DELETE FROM " + entityName + " WHERE uuid = :uuid";

               {
                   QSqlQuery query(database);
                   query.prepare(queryStr);
                   query.bindValue(":uuid", entity.uuid().toString());

                   // Execute the DELETE statement with the entity UUID
                   if (!query.exec())
                   {
                       return Result<T>(
                           Error("SkribFile", Error::Critical, "sql_error", query.lastError().text(), queryStr));
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
               return Result<T>(Error("SkribFile", Error::Fatal, "normaly_unreacheable", ""));
           })
        .withArguments(std::move(entity))
        .onThreadPool(m_databaseContext->threadPool())
        .spawn()
        .result();
}

//--------------------------------------------

template <class T> Result<T> InternalDatabase<T>::add(T &&entity)
{

    return /*WaitInEventLoop::basicWaitInEventLoop<T>(*/
        QtConcurrent::task([this](T entity) {
            // Acquire a write lock before writing to the database

            const QString &entityName = InternalDatabase::getEntityClassName();
            const QStringList &properties = InternalDatabase::getEntityProperties();
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

            QSqlDatabase database = QSqlDatabase::database(m_databaseContext->databaseName());

            {

                int waitTime = 0;

                while (waitTime < 2000000000)
                {
                    waitTime++;
                }

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
                    return Result<T>(
                        Error("SkribFile", Error::Critical, "sql_error", query.lastError().text(), queryStr));
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
            return Result<T>(Error("SkribFile", Error::Fatal, "normaly_unreacheable", ""));
        })
            .withArguments(std::move(entity))
            .onThreadPool(m_databaseContext->threadPool())
            .spawn()
            .result();
}

//--------------------------------------------

template <class T> Result<T> InternalDatabase<T>::update(T &&entity)
{
    return QtConcurrent::task([this](T entity) {
               const QString &entityName = InternalDatabase::getEntityClassName();
               const QStringList &properties = InternalDatabase::getEntityProperties();
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

               QSqlDatabase database = QSqlDatabase::database(m_databaseContext->databaseName());

               {

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
                       return Result<T>(
                           Error("SkribFile", Error::Critical, "sql_error", query.lastError().text(), queryStr));
                   }

                   // Return an appropriate Result object based on the query execution result
                   if (query.numRowsAffected() == 1)
                   {
                       return Result<T>(std::forward<T>(entity));
                   }
                   else
                   {
                       return Result<T>(Error("SkribFile", Error::Critical, "sql_update_failed",
                                              "Failed to update row in database"));
                   }
               }
               return Result<T>(Error("SkribFile", Error::Fatal, "normaly_unreacheable", ""));
           })
        .withArguments(std::move(entity))
        .onThreadPool(m_databaseContext->threadPool())
        .spawn()
        .result();
}

//--------------------------------------------

template <class T> Result<bool> InternalDatabase<T>::exists(const QUuid &uuid)
{
    return QtConcurrent::task([this](QUuid uuid) {
               const QString &entityName = InternalDatabase::getEntityClassName();
               QSqlDatabase database = QSqlDatabase::database(m_databaseContext->databaseName());

               {

                   QSqlQuery query(database);
                   QString queryStr = "SELECT COUNT(*) FROM " + entityName + " WHERE uuid = :uuid";
                   query.prepare(queryStr);
                   query.bindValue(":uuid", uuid.toString());
                   query.exec();

                   if (query.lastError().isValid())
                   {
                       return Result<bool>(
                           Error("SkribFile", Error::Critical, "sql_error", query.lastError().text(), queryStr));
                   }

                   if (query.next())
                   {
                       return Result<bool>(query.value(0).toBool());
                   }
                   else
                   {
                       return Result<bool>(Error("SkribFile", Error::Critical, "sql_row_missing",
                                                 "No row with uuid " + uuid.toString()));
                   }
               }
               return Result<bool>(Error("SkribFile", Error::Fatal, "normaly_unreacheable", ""));
           })
        .withArguments(uuid)
        .onThreadPool(m_databaseContext->threadPool())
        .spawn()
        .result();
}

//--------------------------------------------

template <class T> QString InternalDatabase<T>::getEntityClassName()
{

    const QMetaObject &sourceMetaObject = T::staticMetaObject;

    return QString(sourceMetaObject.className()).split("::").last();
}

//--------------------------------------------

template <class T> QStringList InternalDatabase<T>::getEntityProperties()
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

template <class T> Result<T> InternalDatabase<T>::mapToEntity(const QHash<QString, QVariant> &fieldWithValue)
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

} // namespace Database
