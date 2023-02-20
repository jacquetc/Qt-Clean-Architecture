#include "author_repository.h"

using namespace Repository;

AuthorRepository::AuthorRepository(QObject *parent) : QObject(parent), Repository::GenericRepository<Domain::Author>()
{
}
