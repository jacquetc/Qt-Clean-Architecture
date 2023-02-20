#pragma once

#include "database/interface_database_context.h"
#include "result.h"
#include <QUrl>

class SkribFileContext : public Contracts::Database::InterfaceDatabaseContext
{
  public:
    explicit SkribFileContext(const QUrl &fileName);
    Result<void *> init();

    QString databaseName() const;

    QUrl fileName() const;
    void setFileName(const QUrl &newFileName);

  signals:

  private:
    QUrl m_fileName;
    QString m_databaseName;
    Result<QString> loadSkribFile(const QUrl &fileName) const;
};
