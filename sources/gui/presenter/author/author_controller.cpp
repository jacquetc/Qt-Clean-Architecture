#include "author_controller.h"

#include "QtConcurrent/qtconcurrentrun.h"
#include "author_undo_commands.h"
#include "cqrs/author/commands/create_author_command.h"
#include "cqrs/author/commands/update_author_command.h"
#include "features/author/handlers/queries/get_author_list_request_handler.h"
#include "features/author/handlers/queries/get_author_request_handler.h"
#include "scoped_command_queue.h"

using namespace Presenter::Author;
using namespace Application::Features::Author::Queries;
using namespace Contracts::CQRS::Author::Commands;

QScopedPointer<AuthorController> AuthorController::s_instance = QScopedPointer<AuthorController>(nullptr);

AuthorController::AuthorController(InterfaceRepositoryProvider *repositoryProvider)
    : QObject(nullptr), m_repositoryProvider(repositoryProvider), m_signal_bridge(new AuthorSignalBridge(this))
{
    // connections for undo commands:
    connect(m_signal_bridge, &AuthorSignalBridge::authorCreated, this, &AuthorController::authorCreated);
    connect(m_signal_bridge, &AuthorSignalBridge::authorRemoved, this, &AuthorController::authorRemoved);
    connect(m_signal_bridge, &AuthorSignalBridge::authorUpdated, this, &AuthorController::authorUpdated);

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

    auto interface = qSharedPointerCast<InterfaceAuthorRepository>(
        m_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    GetAuthorRequestHandler handler(interface);
    connect(&handler, &GetAuthorRequestHandler::getAuthorReplied, this, &AuthorController::getAuthorReplied);
    handler.handleAsync(request);
}

Result<AuthorDTO> AuthorController::get(const QUuid &uuid)
{
    GetAuthorRequest request;
    request.id = uuid;

    auto interface = qSharedPointerCast<InterfaceAuthorRepository>(
        m_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    GetAuthorRequestHandler handler(interface);
    return handler.handle(request);
}

void AuthorController::getAllAsync()
{
    auto interface = qSharedPointerCast<InterfaceAuthorRepository>(
        m_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    GetAuthorListRequestHandler handler(interface);
    connect(&handler, &GetAuthorListRequestHandler::getAuthorListReplied, this,
            &AuthorController::getAuthorListReplied);

    handler.handleAsync();
}

Result<QList<AuthorDTO>> AuthorController::getAll()
{
    auto interface = qSharedPointerCast<InterfaceAuthorRepository>(
        m_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    GetAuthorListRequestHandler handler(interface);
    return handler.handle();
}

void AuthorController::createAsync(const CreateAuthorDTO &dto)
{

    CreateAuthorCommand request;
    request.req = dto;

    auto repository = qSharedPointerCast<InterfaceAuthorRepository>(
        m_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    auto command = new CreateUndoCommand(m_signal_bridge, repository, request);
    ScopedCommandQueue::enqueueCommand(command, ScopedCommandQueue::Scope::Author);
}

Result<QUuid> AuthorController::create(const CreateAuthorDTO &dto)
{
    QWaitCondition condition;
    QMutex mutex;
    Result<QUuid> finalResult;

    CreateAuthorCommand request;
    request.req = dto;

    auto repository = qSharedPointerCast<InterfaceAuthorRepository>(
        m_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    auto command = new CreateUndoCommand(m_signal_bridge, repository, request);

    QObject::connect(
        m_signal_bridge, &AuthorSignalBridge::authorCreated, this,
        [this, &condition, &mutex, &finalResult](Result<QUuid> result) {
            finalResult = result;
            mutex.lock();
            condition.wakeAll();
            mutex.unlock();
        },
        Qt::QueuedConnection);

    ScopedCommandQueue::enqueueCommand(command, ScopedCommandQueue::Scope::Author);

    mutex.lock();
    condition.wait(&mutex);
    mutex.unlock();

    return finalResult;
}

void AuthorController::updateAsync(const UpdateAuthorDTO &dto)
{
    UpdateAuthorCommand request;
    request.req = dto;

    auto repository = qSharedPointerCast<InterfaceAuthorRepository>(
        m_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    auto command = new UpdateUndoCommand(m_signal_bridge, repository, request, true);
    ScopedCommandQueue::enqueueCommand(command, ScopedCommandQueue::Scope::Author);
}

Result<AuthorDTO> AuthorController::update(const UpdateAuthorDTO &dto)
{
    UpdateAuthorCommand request;
    request.req = dto;

    auto interface = qSharedPointerCast<InterfaceAuthorRepository>(
        m_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    auto command = new UpdateUndoCommand(m_signal_bridge, interface, request, false);

    ScopedCommandQueue::enqueueCommand(command, ScopedCommandQueue::Scope::Author);
    return command->result();
}

void AuthorController::removeAsync(const QUuid &uuid)
{
    RemoveAuthorCommand request;
    request.id = uuid;

    auto interface = qSharedPointerCast<InterfaceAuthorRepository>(
        m_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    auto command = new RemoveUndoCommand(m_signal_bridge, interface, request, true);
    ScopedCommandQueue::enqueueCommand(command, ScopedCommandQueue::Scope::Author);
}

Result<AuthorDTO> AuthorController::remove(const QUuid &uuid)
{
    RemoveAuthorCommand request;
    request.id = uuid;

    auto interface = qSharedPointerCast<InterfaceAuthorRepository>(
        m_repositoryProvider->repository(InterfaceRepositoryProvider::Author));

    auto command = new RemoveUndoCommand(m_signal_bridge, interface, request, false);
    ScopedCommandQueue::enqueueCommand(command, ScopedCommandQueue::Scope::Author);
    return command->result();
}
