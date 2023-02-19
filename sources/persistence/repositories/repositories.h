#ifndef REPOSITORIES_H
#define REPOSITORIES_H

#include <QHash>
#include <QObject>

#include "database/interface_database_context.h"
#include "persistence/interface_repositories.h"
#include "persistence/interface_repository.h"

using namespace Contracts::Database;
using namespace Contracts::Persistence;

namespace Repository
{
class Repositories : public InterfaceRepositories
{

  public:
    static Repositories *instance();
    ~Repositories();

    InterfaceRepository *get(InterfaceRepositories::Entities entity) override;
    void append(InterfaceRepositories::Entities entity, InterfaceRepository *repository) override;
    void setDatabaseContext(InterfaceDatabaseContext *context) override;

  private:
    explicit Repositories();
    static Repositories *s_instance;
    QHash<InterfaceRepositories::Entities, InterfaceRepository *> m_repositories;
    InterfaceDatabaseContext *m_context = nullptr;

    Repositories(const Repositories &) = delete;
    Repositories &operator=(const Repositories &) = delete;
  signals:
};
} // namespace Repository

#endif // REPOSITORIES_H
