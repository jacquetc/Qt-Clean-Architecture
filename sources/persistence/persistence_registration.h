#ifndef PERSISTENCEREGISTRATION_H
#define PERSISTENCEREGISTRATION_H

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

#endif // PERSISTENCEREGISTRATION_H
