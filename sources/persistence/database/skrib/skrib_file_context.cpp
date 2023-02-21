#include "skrib_file_context.h"
#include "QtConcurrent/qtconcurrenttask.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QTemporaryFile>

SkribFileContext::SkribFileContext(const QUrl &fileName) : QObject(), m_fileName(fileName)
{
    m_threadPool.setMaxThreadCount(1);
    m_threadPool.setExpiryTimeout(0);
}

SkribFileContext::~SkribFileContext()
{
}

//-------------------------------------------------

Result<void *> SkribFileContext::init()
{

    Result<QString> databaseNameResult = loadSkribFile(m_fileName);

    if (databaseNameResult.isError())
    {
        return Result<void *>(databaseNameResult.error());
    }

    m_databaseName = databaseNameResult.value();

    return Result<void *>(nullptr);
}

//-------------------------------------------------

QString SkribFileContext::databaseName() const
{
    return m_databaseName;
}

//-------------------------------------------------

QUrl SkribFileContext::fileName() const
{
    return m_fileName;
}

//-------------------------------------------------

void SkribFileContext::setFileName(const QUrl &newFileName)
{
    m_fileName = newFileName;
}

//-------------------------------------------------

Result<QString> SkribFileContext::loadSkribFile(const QUrl &fileName)
{
    return waitInEventLoop<QString>(
        QtConcurrent::task([this](const QUrl &fileName) {
            // create a temporary file to copy the database to
            QTemporaryFile tempFile;
            tempFile.open();
            tempFile.setAutoRemove(false);
            QString tempFileName = tempFile.fileName();

            // copy db file to temp
            QString fileNameString;

            // check the file scheme, and create a valid file name string
            if (fileName.scheme() == "qrc")
            {
                fileNameString = fileName.toString(QUrl::RemoveScheme);
                fileNameString = ":" + fileNameString;
            }
            else if (fileName.path().at(2) == ':')
            { // means Windows
                fileNameString = fileName.path().remove(0, 1);
            }
            else
            {
                fileNameString = fileName.path();
            }

            // open the original database file and copy its contents to the temporary file
            QFile file(fileNameString);

            if (!file.exists())
            {
                return Result<QString>(Error("SkribFileContext", Error::Critical, "absent_filename",
                                             fileNameString + " doesn't exist", fileNameString));
            }

            if (!file.open(QIODevice::ReadOnly))
            {
                return Result<QString>(Error("SkribFileContext", Error::Critical, "readonly_filename",
                                             fileNameString + " can't be opened", fileNameString));
            }

            QByteArray array(file.readAll());
            tempFile.write(array);
            tempFile.close();

            // open temp file
            QSqlDatabase sqlDb = QSqlDatabase::addDatabase("QSQLITE", tempFileName);
            sqlDb.setHostName("localhost");
            sqlDb.setDatabaseName(tempFileName);

            // try to open the copied database file
            bool ok = sqlDb.open();

            if (!ok)
            {
                return Result<QString>(Error("SkribFileContext", Error::Critical, "cant_open_database",
                                             "Can't open database " + tempFileName, tempFileName));
            }

            // database optimization options
            QStringList optimization;
            optimization << QStringLiteral("PRAGMA case_sensitive_like=true")
                         << QStringLiteral("PRAGMA journal_mode=MEMORY") << QStringLiteral("PRAGMA temp_store=MEMORY")
                         << QStringLiteral("PRAGMA locking_mode=EXCLUSIVE")
                         << QStringLiteral("PRAGMA synchronous = OFF")
                         << QStringLiteral("PRAGMA recursive_triggers=true");

            // start a transaction and execute each optimization option as a separate query
            sqlDb.transaction();

            // TODO: upgrade :
            //     Result result = Upgrader::upgradeSQLite(tempFileName));
            //     if (result)
            //     {
            //         return Result<QString>(
            //             Error(this, Error::Critical, "upgrade_sqlite_failed", "Upgrade of database failed",
            //             tempFileName));
            //     }

            for (const QString &string : qAsConst(optimization))
            {
                QSqlQuery query(sqlDb);

                query.prepare(string);
                query.exec();
            }

            sqlDb.commit();

            // return the name of the copied database file
            return Result<QString>(sqlDb.databaseName());
        })
            .withArguments(fileName)
            .onThreadPool(m_threadPool)
            .spawn());
}

QThreadPool &SkribFileContext::threadPool()
{
    return m_threadPool;
}
