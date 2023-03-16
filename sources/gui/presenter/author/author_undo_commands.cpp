#include "author_undo_commands.h"
#include "automapper/automapper.h"
#include "cqrs/author/commands/remove_author_command.h"
#include "cqrs/author/requests/get_author_request.h"
#include "dto/author/author_dto.h"
#include "features/author/handlers/commands/create_author_command_handler.h"
#include "features/author/handlers/commands/remove_author_command_handler.h"
#include "features/author/handlers/commands/update_author_command_handler.h"
#include "features/author/handlers/queries/get_author_request_handler.h"

using namespace Presenter::Author;
using namespace Presenter::Author::Private;
using namespace Application::Features::Author::Commands;
using namespace Contracts::CQRS::Author::Commands;
using namespace Application::Features::Author::Queries;
using namespace Presenter::UndoRedo;

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

CreateAuthorUndoCommand::CreateAuthorUndoCommand(Private::AuthorSignalBridge *signal_bridge,
                                                 QSharedPointer<InterfaceAuthorRepository> repository,
                                                 const CreateAuthorCommand &request)
    : UndoRedoCommand(QObject::tr("Create author")), m_signalBridge(signal_bridge), m_repository(repository),
      m_request(request)
{
}

Result<void> CreateAuthorUndoCommand::undo()
{
    RemoveAuthorCommand removeRequest;
    removeRequest.id = m_result.value();

    RemoveAuthorCommandHandler handler(m_repository);

    // do
    auto result = handler.handle(removeRequest);
    if (result.isSuccess())
    {
        emit m_signalBridge->authorRemoved(result);
    }
    return Result<void>(result.error());
}

Result<void> CreateAuthorUndoCommand::redo()
{
    if (!m_result.isEmpty())
    {
        m_request.req.setUuid(m_result.value());
    }

    CreateAuthorCommandHandler handler(m_repository);
    m_result = handler.handle(m_request);
    if (m_result.isSuccess())
    {
        emit m_signalBridge->authorCreated(m_result);
    }
    return Result<void>(m_result.error());
}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

UpdateAuthorUndoCommand::UpdateAuthorUndoCommand(Private::AuthorSignalBridge *signal_bridge,
                                                 QSharedPointer<InterfaceAuthorRepository> repository,
                                                 const UpdateAuthorCommand &request)
    : UndoRedoCommand(QObject::tr("Update author")), m_signalBridge(signal_bridge), m_repository(repository),
      m_request(request)
{
}

Result<void> UpdateAuthorUndoCommand::undo()
{

    UpdateAuthorCommand command;
    command.req = AutoMapper::AutoMapper::map<UpdateAuthorDTO>(m_oldState.value());

    UpdateAuthorCommandHandler handler(m_repository);

    // do
    auto result = handler.handle(command);
    if (result.isSuccess())
    {
        emit m_signalBridge->authorUpdated(result);
    }
    return Result<void>(result.error());
}

Result<void> UpdateAuthorUndoCommand::redo()
{

    // save old state
    if (m_oldState.isEmpty())
    {
        GetAuthorRequestHandler getHandler(m_repository);
        GetAuthorRequest getRequest;
        getRequest.id = m_request.req.uuid();
        m_oldState = getHandler.handle(getRequest);
        if (m_oldState.hasError())
        {
            return Result<void>(m_oldState.error());
        }
    }

    // do
    UpdateAuthorCommandHandler handler(m_repository);
    auto result = handler.handle(m_request);
    if (result.isSuccess())
    {
        emit m_signalBridge->authorUpdated(result);
    }
    return Result<void>(result.error());
}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

RemoveAuthorUndoCommand::RemoveAuthorUndoCommand(Private::AuthorSignalBridge *signal_bridge,
                                                 QSharedPointer<InterfaceAuthorRepository> repository,
                                                 const RemoveAuthorCommand &request)
    : UndoRedoCommand(QObject::tr("Remove author")), m_signalBridge(signal_bridge), m_repository(repository),
      m_request(request)
{
}

Result<void> RemoveAuthorUndoCommand::undo()
{

    CreateAuthorCommand command;
    command.req = AutoMapper::AutoMapper::map<CreateAuthorDTO>(m_result.value());

    CreateAuthorCommandHandler handler(m_repository);

    auto result = handler.handle(command);
    if (result.isSuccess())
    {
        emit m_signalBridge->authorCreated(result);
    }
    return Result<void>(result.error());
}

Result<void> RemoveAuthorUndoCommand::redo()
{

    // do
    RemoveAuthorCommandHandler handler(m_repository);
    auto result = handler.handle(m_request);
    if (result.isSuccess())
    {
        emit m_signalBridge->authorRemoved(result);
    }
    return Result<void>(result.error());
}

Result<AuthorDTO> RemoveAuthorUndoCommand::result() const
{
    return m_result;
}
