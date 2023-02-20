#pragma once

#include "author.h"
#include "generic_repository.h"
#include "persistence/interface_author_repository.h"
#include "persistence_global.h"
#include <QObject>

namespace Repository
{
class SKRPERSISTENCEEXPORT AuthorRepository : public QObject,
                                              public Repository::GenericRepository<Domain::Author>,
                                              public Contracts::Persistence::InterfaceAuthorRepository
{
    Q_OBJECT
  public:
    explicit AuthorRepository(QObject *parent = nullptr);

  signals:
    void progressChanged(int minimum, int maximum, int value) override;
};

} // namespace Repository
