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

AuthorController *AuthorController::instance()
{
    return s_instance.data();
}

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

Result<AuthorDTO> AuthorController::get(const QUuid &uuid)
{
    GetAuthorRequest request;
    request.id = uuid;

    auto interface = qSharedPointerCast<InterfaceAuthorRepository>(
        s_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    GetAuthorRequestHandler handler(interface);
    return handler.handle(request);
}

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

Result<QList<AuthorDTO>> AuthorController::getAll()
{
    auto interface = qSharedPointerCast<InterfaceAuthorRepository>(
        s_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    GetAuthorListRequestHandler handler(interface);
    return handler.handle();
}

void AuthorController::createAsync(const CreateAuthorDTO &dto)
{
    CreateAuthorCommand request;
    request.req = dto;

    auto repository = qSharedPointerCast<InterfaceAuthorRepository>(
        s_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    auto command = new CreateUndoCommand(s_signal_bridge, repository, request);
    s_undo_redo_system->push(command, UndoRedoCommand::Scope::Author);
}

void AuthorController::updateAsync(const UpdateAuthorDTO &dto)
{
    UpdateAuthorCommand request;
    request.req = dto;

    auto repository = qSharedPointerCast<InterfaceAuthorRepository>(
        s_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    auto command = new UpdateUndoCommand(s_signal_bridge, repository, request);
    s_undo_redo_system->push(command, UndoRedoCommand::Scope::Author);
}

void AuthorController::removeAsync(const QUuid &uuid)
{
    RemoveAuthorCommand request;
    request.id = uuid;

    auto interface = qSharedPointerCast<InterfaceAuthorRepository>(
        s_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    auto command = new RemoveUndoCommand(s_signal_bridge, interface, request);
    s_undo_redo_system->push(command, UndoRedoCommand::Scope::Author);
}
