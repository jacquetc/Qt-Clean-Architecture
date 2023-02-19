#include "skrib_file_context.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QTemporaryFile>

SkribFileContext::SkribFileContext(const QUrl &fileName) : m_fileName(fileName)
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

Result<QString> SkribFileContext::loadSkribFile(const QUrl &fileName) const
{
    // create temp file
    QTemporaryFile tempFile;
    tempFile.open();
    tempFile.setAutoRemove(false);
    QString tempFileName = tempFile.fileName();

    // copy db file to temp
    QString fileNameString;

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

    // QSqlDatabase memoryDb = copySQLiteDbToMemory(sqlDb, projectId);
    //        QSqlDatabase db = QSqlDatabase::database("db_to_be_imported",
    // false);
    //        db.removeDatabase("db_to_be_imported");
    bool ok = sqlDb.open();

    if (!ok)
    {
        return Result<QString>(Error("SkribFileContext", Error::Critical, "cant_open_database",
                                     "Can't open database " + tempFileName, tempFileName));
    }

    // optimization :
    QStringList optimization;
    optimization << QStringLiteral("PRAGMA case_sensitive_like=true") << QStringLiteral("PRAGMA journal_mode=MEMORY")
                 << QStringLiteral("PRAGMA temp_store=MEMORY") << QStringLiteral("PRAGMA locking_mode=EXCLUSIVE")
                 << QStringLiteral("PRAGMA synchronous = OFF") << QStringLiteral("PRAGMA recursive_triggers=true");
    sqlDb.transaction();

    // TODO: upgrade :
    //     Result result = Upgrader::upgradeSQLite(tempFileName));
    //     if (result)
    //     {
    //         return Result<QString>(
    //             Error(this, Error::Critical, "upgrade_sqlite_failed", "Upgrade of database failed", tempFileName));
    //     }

    for (const QString &string : qAsConst(optimization))
    {
        QSqlQuery query(sqlDb);

        query.prepare(string);
        query.exec();
    }

    sqlDb.commit();

    return Result<QString>(sqlDb.databaseName());
}
