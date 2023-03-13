#include "author_undo_commands.h"
#include "automapper/automapper.h"
#include "cqrs/author/commands/remove_author_command.h"
#include "cqrs/author/requests/get_author_request.h"
#include "dto/author/author_dto.h"
#include "features/author/handlers/commands/create_author_command_handler.h"
#include "features/author/handlers/commands/remove_author_command_handler.h"
#include "features/author/handlers/commands/update_author_command_handler.h"
#include "features/author/handlers/queries/get_author_request_handler.h"
#include "scoped_command_queue.h"

using namespace Presenter::Author;
using namespace Presenter::Author::Private;
using namespace Application::Features::Author::Commands;
using namespace Contracts::CQRS::Author::Commands;
using namespace Application::Features::Author::Queries;

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

CreateUndoCommand::CreateUndoCommand(Private::AuthorSignalBridge *signal_bridge,
                                     QSharedPointer<InterfaceAuthorRepository> repository,
                                     const CreateAuthorCommand &request)
    : QUndoCommand(QObject::tr("Create author")), m_signalBridge(signal_bridge), m_repository(repository),
      m_request(request), m_handler(new CreateAuthorCommandHandler(m_repository))
{

    // connect to signal
    m_signalBridge->connect(m_handler, &CreateAuthorCommandHandler::authorCreated, m_signalBridge,
                            &AuthorSignalBridge::authorCreated, Qt::DirectConnection);

    // connect to signal
    m_signalBridge->connect(
        m_handler, &CreateAuthorCommandHandler::authorCreated, m_signalBridge,
        [this](Result<QUuid> result) {
            this->setObsolete(result.isError());
            ScopedCommandQueue::commandFinished(ScopedCommandQueue::Scope::Author);
            m_mutex.lock();
            m_condition.wakeAll();
            m_mutex.unlock();
        },
        Qt::QueuedConnection);
}

void CreateUndoCommand::undo()
{
    RemoveAuthorCommand removeRequest;
    removeRequest.id = m_result.value();

    RemoveAuthorCommandHandler handler(m_repository);
    // connect to signal
    m_signalBridge->connect(&handler, &RemoveAuthorCommandHandler::authorRemoved, m_signalBridge,
                            &AuthorSignalBridge::authorRemoved);

    // do
    auto result = handler.handle(removeRequest);
    this->setObsolete(result.isError());

    ScopedCommandQueue::commandFinished(ScopedCommandQueue::Scope::Author);
}

void CreateUndoCommand::redo()
{
    m_handler->handleAsync(m_request);

    m_mutex.lock();
    m_condition.wait(&m_mutex);
    m_mutex.unlock();
}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

UpdateUndoCommand::UpdateUndoCommand(Private::AuthorSignalBridge *signal_bridge,
                                     QSharedPointer<InterfaceAuthorRepository> repository,
                                     const UpdateAuthorCommand &request, bool async)
    : QUndoCommand(QObject::tr("Update author")), m_signalBridge(signal_bridge), m_repository(repository),
      m_request(request), m_async(async), m_handler(new UpdateAuthorCommandHandler(m_repository))
{

    // connect to signal
    m_signalBridge->connect(m_handler, &UpdateAuthorCommandHandler::authorUpdated, m_signalBridge,
                            &AuthorSignalBridge::authorUpdated);

    if (m_async)
    {
        // connect to signal
        m_signalBridge->connect(m_handler, &UpdateAuthorCommandHandler::authorUpdated, m_signalBridge,
                                []() { ScopedCommandQueue::commandFinished(ScopedCommandQueue::Scope::Author); });
    }
}

void UpdateUndoCommand::undo()
{

    UpdateAuthorCommand command;
    command.req = AutoMapper::AutoMapper::map<UpdateAuthorDTO>(m_oldState.value());

    UpdateAuthorCommandHandler handler(m_repository);
    // connect to signal
    m_signalBridge->connect(&handler, &UpdateAuthorCommandHandler::authorUpdated, m_signalBridge,
                            &AuthorSignalBridge::authorUpdated);

    // do
    auto result = handler.handle(command);
    this->setObsolete(result.isError());
    ScopedCommandQueue::commandFinished(ScopedCommandQueue::Scope::Author);
}

void UpdateUndoCommand::redo()
{

    // save old state
    if (m_oldState.isEmpty())
    {
        GetAuthorRequestHandler getHandler(m_repository);
        GetAuthorRequest getRequest;
        getRequest.id = m_request.req.uuid();
        Result<AuthorDTO> old = getHandler.handle(getRequest);
        if (m_oldState.hasError())
        {
            m_result = old;
            this->setObsolete(true);
            return;
        }
    }

    // do
    if (m_async)
    {
        m_handler->handleAsync(m_request);
    }
    else
    {
        m_result = m_handler->handle(m_request);
        this->setObsolete(m_result.isError());
        ScopedCommandQueue::commandFinished(ScopedCommandQueue::Scope::Author);
    }
}

Result<AuthorDTO> UpdateUndoCommand::result() const
{
    return m_result;
}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

RemoveUndoCommand::RemoveUndoCommand(Private::AuthorSignalBridge *signal_bridge,
                                     QSharedPointer<InterfaceAuthorRepository> repository,
                                     const RemoveAuthorCommand &request, bool async)
    : QUndoCommand(QObject::tr("Remove author")), m_signalBridge(signal_bridge), m_repository(repository),
      m_request(request), m_async(async), m_handler(new RemoveAuthorCommandHandler(m_repository))
{

    // connect to signal
    m_signalBridge->connect(m_handler, &RemoveAuthorCommandHandler::authorRemoved, m_signalBridge,
                            &AuthorSignalBridge::authorRemoved);

    if (m_async)
    {
        // connect to signal
        m_signalBridge->connect(m_handler, &RemoveAuthorCommandHandler::authorRemoved, m_signalBridge,
                                []() { ScopedCommandQueue::commandFinished(ScopedCommandQueue::Scope::Author); });
    }
}

void RemoveUndoCommand::undo()
{

    CreateAuthorCommand command;
    command.req = AutoMapper::AutoMapper::map<CreateAuthorDTO>(m_result.value());

    CreateAuthorCommandHandler handler(m_repository);
    // connect to signal
    m_signalBridge->connect(&handler, &CreateAuthorCommandHandler::authorCreated, m_signalBridge,
                            &AuthorSignalBridge::authorCreated);
    handler.handle(command);
    this->setObsolete(m_result.isError());

    ScopedCommandQueue::commandFinished(ScopedCommandQueue::Scope::Author);
}

void RemoveUndoCommand::redo()
{

    // do
    if (m_async)
    {
        m_handler->handleAsync(m_request);
    }
    else
    {
        m_result = m_handler->handle(m_request);
        this->setObsolete(m_result.isError());
        ScopedCommandQueue::commandFinished(ScopedCommandQueue::Scope::Author);
    }
}

Result<AuthorDTO> RemoveUndoCommand::result() const
{
    return m_result;
}
