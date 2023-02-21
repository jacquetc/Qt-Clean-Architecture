#pragma once

#include "database/interface_database_context.h"
#include "persistence_global.h"
#include "result.h"
#include "wait_in_event_loop.h"
#include <QThreadPool>
#include <QUrl>

/**
 * @brief The SkribFileContext class represents the context for a Skrib file database.
 */
class SKRPERSISTENCEEXPORT SkribFileContext : public QObject,
                                              public Contracts::Database::InterfaceDatabaseContext,
                                              public WaitInEventLoop
{
    Q_OBJECT
  public:
    /**
     * @brief Constructs a new SkribFileContext object with the given file name.
     * @param fileName The file name of the Skrib file database.
     */
    explicit SkribFileContext(const QUrl &fileName);

    /**
     * @brief Destructor for the SkribFileContext object.
     */
    ~SkribFileContext();

    /**
     * @brief Initializes the Skrib file database.
     * @return A Result object with a value of nullptr if successful, or an Error object if an error occurred.
     */
    Result<void *> init();

    /**
     * @brief Returns the name of the Skrib file database.
     * @return The name of the Skrib file database.
     */
    QString databaseName() const;

    /**
     * @brief Returns the file name of the Skrib file database.
     * @return The file name of the Skrib file database.
     */
    QUrl fileName() const;

    /**
     * @brief Sets the file name of the Skrib file database.
     * @param newFileName The new file name of the Skrib file database.
     */
    void setFileName(const QUrl &newFileName);

    /**
     * @brief Returns the thread pool used by the SkribFileContext object.
     * Necesssary so the QSqlDatabase can stay on the same thread.
     * @return The thread pool used by the SkribFileContext object.
     */
    QThreadPool &threadPool();

  signals:
    /**
     * @brief A signal that is emitted when the progress of a task changes.
     * @param minimum The minimum progress value.
     * @param maximum The maximum progress value.
     * @param value The current progress value.
     */
    void progressChanged(int minimum, int maximum, int value);

  private:
    QUrl m_fileName;          /**< The file name of the Skrib file database. */
    QString m_databaseName;   /**< The name of the Skrib file database. */
    QThreadPool m_threadPool; /**< The thread pool used for loading the Skrib file database. */

    /**
     * @brief Loads the Skrib file database from the given file name.
     * @param fileName The file name of the Skrib file database to load.
     * @return A Result object with the name of the loaded database if successful, or an Error object if an error
     * occurred.
     */
    Result<QString> loadSkribFile(const QUrl &fileName);
};
