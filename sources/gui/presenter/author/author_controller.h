#pragma once

#include "author_signal_bridge.h"
#include "dto/author/author_dto.h"
#include "dto/author/create_author_dto.h"
#include "dto/author/update_author_dto.h"
#include "persistence/interface_repository_provider.h"
#include "presenter_global.h"
#include "result.h"
#include "undo_redo/threaded_undo_redo_system.h"

using namespace Contracts::DTO::Author;
using namespace Presenter;
using namespace Contracts::Persistence;
using namespace Presenter::Author::Private;
using namespace Presenter::UndoRedo;

namespace Presenter::Author
{

class SKRPRESENTEREXPORT AuthorController : public QObject
{
    Q_OBJECT
  public:
    AuthorController(InterfaceRepositoryProvider *repositoryProvider);

    static AuthorController *instance();

    static void getAsync(const QUuid &uuid);
    Result<AuthorDTO> get(const QUuid &uuid);

    static void getAllAsync();
    Result<QList<AuthorDTO>> getAll();

    static void createAsync(const CreateAuthorDTO &dto);

    static void updateAsync(const UpdateAuthorDTO &dto);

    static void removeAsync(const QUuid &uuid);

  signals:

    void getAuthorReplied(Result<Contracts::DTO::Author::AuthorDTO> result);
    void getAuthorListReplied(Result<QList<Contracts::DTO::Author::AuthorDTO>> result);
    void authorCreated(Result<QUuid> result);
    void authorRemoved(Result<Contracts::DTO::Author::AuthorDTO> result);
    void authorUpdated(Result<Contracts::DTO::Author::AuthorDTO> result);

  private:
    static QScopedPointer<AuthorController> s_instance;
    static AuthorSignalBridge *s_signal_bridge;
    static InterfaceRepositoryProvider *s_repositoryProvider;
    static ThreadedUndoRedoSystem *s_undo_redo_system;
    AuthorController() = delete;
    AuthorController(const AuthorController &) = delete;
    AuthorController &operator=(const AuthorController &) = delete;
};

} // namespace Presenter::Author
