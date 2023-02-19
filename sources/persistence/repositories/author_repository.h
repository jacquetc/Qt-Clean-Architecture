#ifndef AUTHORREPOSITORY_H
#define AUTHORREPOSITORY_H

#include "author.h"
#include "generic_repository.h"
#include "persistence/interface_author_repository.h"
#include "persistence_global.h"

namespace Repository
{
class SKRPERSISTENCEEXPORT AuthorRepository : public Repository::GenericRepository<Domain::Author>,
                                              public Contracts::Persistence::InterfaceAuthorRepository
{

  public:
    explicit AuthorRepository();

  protected:
    void setProgressChanged(int minimum, int maximum, int value) override;

    //  signals:
    //    void progressChanged(int minimum, int maximum, int value);
};

} // namespace Repository
#endif // AUTHORREPOSITORY_H
