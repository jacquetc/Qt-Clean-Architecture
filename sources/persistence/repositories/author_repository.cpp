#include "author_repository.h"

using namespace Repository;

AuthorRepository::AuthorRepository() : Repository::GenericRepository<Domain::Author>()
{
}

void AuthorRepository::setProgressChanged(int minimum, int maximum, int value)
{
    // emit progressChanged(minimum, maximum, value);
}
