#include "author_controller.h"

#include "author_undo_commands.h"
#include "cqrs/author/commands/create_author_command.h"
#include "cqrs/author/commands/update_author_command.h"
#include "features/author/handlers/queries/get_author_list_request_handler.h"
#include "features/author/handlers/queries/get_author_request_handler.h"
#include "undo_redo/query_command.h"

using namespace Presenter::Author;
using namespace Presenter::UndoRedo;
using namespace Application::Features::Author::Queries;
using namespace Contracts::CQRS::Author::Commands;

QScopedPointer<AuthorController> AuthorController::s_instance = QScopedPointer<AuthorController>(nullptr);
AuthorSignalBridge *AuthorController::s_signal_bridge;
InterfaceRepositoryProvider *AuthorController::s_repositoryProvider;
ThreadedUndoRedoSystem *AuthorController::s_undo_redo_system;
/*!
    \class AuthorController
    \brief The AuthorController class provides an asynchronous interface for managing Author objects.

    The AuthorController class provides methods for getting, creating, updating, and removing authors in an
    asynchronous manner. It uses the Command and Query Responsibility Segregation (CQRS) pattern to
    separate read and write operations. The class also implements the Singleton pattern, ensuring that
    only one instance of the controller is active at any time.

    The AuthorSignalBridge class is used to emit signals for Author events, such as authorCreated,
    authorRemoved, and authorUpdated. These signals are connected to the corresponding slots in the
    AuthorController class. AuthorSignalBridge isn't meant to be used directly by the UI, only from
    inside the UndoRedoCommand and QueryCommand, the signals are available directly from AuthorController.

    \section1 Example Usage

    \code
    // Get an Author by ID
    AuthorController::instance()->getAsync(authorId);
    \endcode

    \sa AuthorSignalBridge
*/

/*!
    \brief Constructs an AuthorController with the given \a repositoryProvider.
*/
AuthorController::AuthorController(InterfaceRepositoryProvider *repositoryProvider) : QObject(nullptr)
{
    s_repositoryProvider = repositoryProvider;
    s_signal_bridge = new AuthorSignalBridge(this);
    // connections for undo commands:
    connect(s_signal_bridge, &AuthorSignalBridge::authorCreated, this, &AuthorController::authorCreated);
    connect(s_signal_bridge, &AuthorSignalBridge::authorRemoved, this, &AuthorController::authorRemoved);
    connect(s_signal_bridge, &AuthorSignalBridge::authorUpdated, this, &AuthorController::authorUpdated);
    connect(s_signal_bridge, &AuthorSignalBridge::getAuthorReplied, this, &AuthorController::getAuthorReplied);
    connect(s_signal_bridge, &AuthorSignalBridge::getAuthorListReplied, this, &AuthorController::getAuthorListReplied);

    s_undo_redo_system = ThreadedUndoRedoSystem::instance();

    s_instance.reset(this);
}

/*!
    \brief Returns the singleton instance of the AuthorController.
*/
AuthorController *AuthorController::instance()
{
    return s_instance.data();
}

/*!
    \brief Retrieves an author asynchronously by the given \a uuid.

    When the operation is complete, the getAuthorReplied signal is emitted.
*/
void AuthorController::getAsync(const QUuid &uuid)
{
    GetAuthorRequest request;
    request.id = uuid;

    auto queryCommand = new QueryCommand("get");
    queryCommand->setQueryFunction([&]() {
        auto interface = qSharedPointerCast<InterfaceAuthorRepository>(
            s_repositoryProvider->repository(InterfaceRepositoryProvider::Author));
        GetAuthorRequestHandler handler(interface);
        auto result = handler.handle(request);
        emit s_signal_bridge->getAuthorReplied(result);
    });
    s_undo_redo_system->push(queryCommand, UndoRedoCommand::Scope::Author);
}

/*!
    \brief Retrieves an author synchronously by the given \a uuid.

    Returns a Result object containing the AuthorDTO.
*/
Result<AuthorDTO> AuthorController::get(const QUuid &uuid)
{
    GetAuthorRequest request;
    request.id = uuid;

    auto interface = qSharedPointerCast<InterfaceAuthorRepository>(
        s_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    GetAuthorRequestHandler handler(interface);
    return handler.handle(request);
}

/*!
    \brief Retrieves all authors asynchronously.

    When the operation is complete, the getAuthorListReplied signal is emitted.
*/
void AuthorController::getAllAsync()
{
    auto queryCommand = new QueryCommand("getAll");
    queryCommand->setQueryFunction([&]() {
        auto interface = qSharedPointerCast<InterfaceAuthorRepository>(
            s_repositoryProvider->repository(InterfaceRepositoryProvider::Author));
        GetAuthorListRequestHandler handler(interface);
        auto result = handler.handle();
        emit s_signal_bridge->getAuthorListReplied(result);
    });
    s_undo_redo_system->push(queryCommand, UndoRedoCommand::Scope::Author);
}

/*!
    \brief Retrieves all authors synchronously.

    Returns a Result object containing a QList of AuthorDTOs.
*/
Result<QList<AuthorDTO>> AuthorController::getAll()
{
    auto interface = qSharedPointerCast<InterfaceAuthorRepository>(
        s_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    GetAuthorListRequestHandler handler(interface);
    return handler.handle();
}

/*!
    \brief Creates a new author asynchronously with the given \a dto.

    When the operation is complete, the authorCreated signal is emitted.
*/
void AuthorController::createAsync(const CreateAuthorDTO &dto)
{
    CreateAuthorCommand request;
    request.req = dto;

    auto repository = qSharedPointerCast<InterfaceAuthorRepository>(
        s_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    auto command = new CreateAuthorUndoCommand(s_signal_bridge, repository, request);
    s_undo_redo_system->push(command, UndoRedoCommand::Scope::Author);
}

/*!
    \brief Updates an existing author asynchronously with the given \a dto.

    When the operation is complete, the authorUpdated signal is emitted.
*/
void AuthorController::updateAsync(const UpdateAuthorDTO &dto)
{
    UpdateAuthorCommand request;
    request.req = dto;

    auto repository = qSharedPointerCast<InterfaceAuthorRepository>(
        s_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    auto command = new UpdateAuthorUndoCommand(s_signal_bridge, repository, request);
    s_undo_redo_system->push(command, UndoRedoCommand::Scope::Author);
}

/*!
    \brief Removes an author asynchronously by the given \a uuid.

    When the operation is complete, the authorRemoved signal is emitted.
*/
void AuthorController::removeAsync(const QUuid &uuid)
{
    RemoveAuthorCommand request;
    request.id = uuid;

    auto interface = qSharedPointerCast<InterfaceAuthorRepository>(
        s_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    auto command = new RemoveAuthorUndoCommand(s_signal_bridge, interface, request);
    s_undo_redo_system->push(command, UndoRedoCommand::Scope::Author);
}
