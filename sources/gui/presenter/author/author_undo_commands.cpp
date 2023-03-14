#include "author_undo_commands.h"
#include "QtConcurrent/qtconcurrentmap.h"
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

CreateUndoCommand::CreateUndoCommand(Private::AuthorSignalBridge *signal_bridge,
                                     QSharedPointer<InterfaceAuthorRepository> repository,
                                     const CreateAuthorCommand &request)
    : UndoRedoCommand(QObject::tr("Create author")), m_signalBridge(signal_bridge), m_repository(repository),
      m_request(request)
{
}

void CreateUndoCommand::undo()
{
    RemoveAuthorCommand removeRequest;
    removeRequest.id = m_result.value();

    RemoveAuthorCommandHandler handler(m_repository);

    // do
    auto result = handler.handle(removeRequest);
    this->setObsolete(result.isError());
    emit m_signalBridge->authorRemoved(result);
}

void CreateUndoCommand::redo()
{
    CreateAuthorCommandHandler handler(m_repository);
    Result<QUuid> result = handler.handle(m_request);
    this->setObsolete(result.isError());
    emit m_signalBridge->authorCreated(result);
}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

UpdateUndoCommand::UpdateUndoCommand(Private::AuthorSignalBridge *signal_bridge,
                                     QSharedPointer<InterfaceAuthorRepository> repository,
                                     const UpdateAuthorCommand &request)
    : UndoRedoCommand(QObject::tr("Update author")), m_signalBridge(signal_bridge), m_repository(repository),
      m_request(request)
{
}

void UpdateUndoCommand::undo()
{

    UpdateAuthorCommand command;
    command.req = AutoMapper::AutoMapper::map<UpdateAuthorDTO>(m_oldState.value());

    UpdateAuthorCommandHandler handler(m_repository);

    // do
    auto result = handler.handle(command);
    this->setObsolete(result.isError());
    emit m_signalBridge->authorUpdated(result);
}

void UpdateUndoCommand::redo()
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
            this->setObsolete(true);
            return;
        }
    }

    // do
    UpdateAuthorCommandHandler handler(m_repository);
    auto result = handler.handle(m_request);
    this->setObsolete(result.isError());
    emit m_signalBridge->authorUpdated(result);
}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

RemoveUndoCommand::RemoveUndoCommand(Private::AuthorSignalBridge *signal_bridge,
                                     QSharedPointer<InterfaceAuthorRepository> repository,
                                     const RemoveAuthorCommand &request)
    : UndoRedoCommand(QObject::tr("Remove author")), m_signalBridge(signal_bridge), m_repository(repository),
      m_request(request)
{
}

void RemoveUndoCommand::undo()
{

    CreateAuthorCommand command;
    command.req = AutoMapper::AutoMapper::map<CreateAuthorDTO>(m_result.value());

    CreateAuthorCommandHandler handler(m_repository);

    auto result = handler.handle(command);
    this->setObsolete(result.isError());
    emit m_signalBridge->authorCreated(result);
}

void RemoveUndoCommand::redo()
{

    // do
    RemoveAuthorCommandHandler handler(m_repository);
    auto result = handler.handle(m_request);
    emit m_signalBridge->authorRemoved(result);
    this->setObsolete(m_result.isError());
}

Result<AuthorDTO> RemoveUndoCommand::result() const
{
    return m_result;
}
