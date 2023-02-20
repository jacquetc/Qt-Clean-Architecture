#pragma once

#include "repositories/repositories.h"
#include <QObject>

class PersistenceRegistration : public QObject
{
    Q_OBJECT
  public:
    explicit PersistenceRegistration(QObject *parent);

  signals:
  private:
    QScopedPointer<Repository::Repositories> m_repositories;
};
