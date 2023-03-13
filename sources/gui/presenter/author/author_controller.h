#pragma once

#include "author_signal_bridge.h"
#include "dto/author/author_dto.h"
#include "dto/author/create_author_dto.h"
#include "dto/author/update_author_dto.h"
#include "persistence/interface_repository_provider.h"
#include "presenter_global.h"
#include "result.h"

using namespace Contracts::DTO::Author;
using namespace Presenter;
using namespace Contracts::Persistence;
using namespace Presenter::Author::Private;

namespace Presenter::Author
{

class SKRPRESENTEREXPORT AuthorController : public QObject
{
    Q_OBJECT
  public:
    AuthorController(InterfaceRepositoryProvider *repositoryProvider);

    static AuthorController *instance();

    void getAsync(const QUuid &uuid);
    Result<AuthorDTO> get(const QUuid &uuid);

    void getAllAsync();
    Result<QList<AuthorDTO>> getAll();

    void createAsync(const CreateAuthorDTO &dto);
    Result<QUuid> create(const CreateAuthorDTO &dto);

    void updateAsync(const UpdateAuthorDTO &dto);
    Result<AuthorDTO> update(const UpdateAuthorDTO &dto);

    void removeAsync(const QUuid &uuid);
    Result<AuthorDTO> remove(const QUuid &uuid);

  signals:
    void getAuthorReplied(Result<Contracts::DTO::Author::AuthorDTO> result);
    void getAuthorListReplied(Result<QList<Contracts::DTO::Author::AuthorDTO>> result);
    void authorCreated(Result<QUuid> result);
    void authorRemoved(Result<Contracts::DTO::Author::AuthorDTO> result);
    void authorUpdated(Result<Contracts::DTO::Author::AuthorDTO> result);

  private:
    static QScopedPointer<AuthorController> s_instance;
    AuthorSignalBridge *m_signal_bridge;
    InterfaceRepositoryProvider *m_repositoryProvider;
    AuthorController() = delete;
    AuthorController(const AuthorController &) = delete;
    AuthorController &operator=(const AuthorController &) = delete;
};

} // namespace Presenter::Author
