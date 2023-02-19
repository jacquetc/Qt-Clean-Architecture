#ifndef DUMMY_REPOSITORIES_H
#define DUMMY_REPOSITORIES_H

#include "QtCore/qdebug.h"
#include "persistence/interface_repositories.h"
#include <QHash>

class DummyRepositories : public Contracts::Persistence::InterfaceRepositories
{

    // InterfaceRepositories interface
  public:
    void append(Entities entity, InterfaceRepository *repository) override;
    InterfaceRepository *get(Entities entity) override;
    void setDatabaseContext(InterfaceDatabaseContext *context) override;

  private:
    QHash<InterfaceRepositories::Entities, InterfaceRepository *> m_repositories;
};

void DummyRepositories::append(Entities entity, InterfaceRepository *repository)
{

    if (m_repositories.contains(entity))
    {
        qWarning() << "Repositories: m_repositories contains already this InterfaceRepository";
        return;
    }
    m_repositories.insert(entity, repository);
}

InterfaceRepository *DummyRepositories::get(Entities entity)
{

    if (m_repositories.contains(entity))
    {

        return m_repositories.value(entity);
    }
    qFatal("Repositories: Repository missing with Entities %u", entity);

    return nullptr;
}

void DummyRepositories::setDatabaseContext(InterfaceDatabaseContext *context)
{
}

#endif // DUMMY_REPOSITORIES_H
