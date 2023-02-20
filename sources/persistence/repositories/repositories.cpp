#include "repositories.h"
#include "QtCore/qdebug.h"

using namespace Repository;

Repositories::Repositories()
{
}
Repositories *Repositories::s_instance = nullptr;

Repositories *Repositories::instance()
{
    if (!s_instance)
    {
        s_instance = new Repositories;
    }
    return s_instance;
}

Repositories::~Repositories()
{
    qDeleteAll(m_repositories);
    m_repositories.clear();
}

InterfaceRepository *Repositories::get(Entities entity)
{

    QReadLocker locker(&m_databaseLock); // Acquire a read lock before reading from the database
    if (m_repositories.contains(entity))
    {

        return m_repositories.value(entity);
    }
    qFatal("Repositories: Repository missing with Entities %u", entity);

    return nullptr;
}

void Repositories::append(Entities entity, InterfaceRepository *repository)
{

    QWriteLocker locker(&m_databaseLock); // Acquire a write lock before writing to the database
    if (m_repositories.contains(entity))
    {
        qWarning() << "Repositories: m_repositories contains already this InterfaceRepository";
        return;
    }
    m_repositories.insert(entity, repository);
}

void Repositories::setDatabaseContext(InterfaceDatabaseContext *context)
{
    QWriteLocker locker(&m_databaseLock); // Acquire a write lock before writing to the database
    m_context = context;
    for (auto repository : qAsConst(m_repositories))
    {
        repository->setDatabaseContext(context);
    }
}
