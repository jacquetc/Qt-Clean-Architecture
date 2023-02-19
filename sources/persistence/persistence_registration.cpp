#include "persistence_registration.h"
#include "repositories/author_repository.h"

using namespace Repository;

PersistenceRegistration::PersistenceRegistration(QObject *parent) : QObject{parent}
{
    m_repositories.reset(Repositories::instance());

    m_repositories->append(Repositories::Entities::Author, new AuthorRepository);
}
