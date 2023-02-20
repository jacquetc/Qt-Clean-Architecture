#include "author_undo_commands.h"
#include "automapper/automapper.h"
#include "cqrs/author/commands/remove_author_command.h"
#include "cqrs/author/requests/get_author_request.h"
#include "dto/author/author_dto.h"
#include "features/author/handlers/commands/create_author_command_handler.h"
#include "features/author/handlers/commands/remove_author_command_handler.h"
#include "features/author/handlers/commands/update_author_command_handler.h"
#include "features/author/handlers/queries/get_author_request_handler.h"
#include "repositories/repositories.h"

using namespace Presenter::Author;
using namespace Application::Features::Author::Commands;
using namespace Contracts::CQRS::Author::Commands;
using namespace Application::Features::Author::Queries;

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

CreateUndoCommand::CreateUndoCommand(const CreateAuthorCommand &request)
    : QUndoCommand(QObject::tr("Create author")), m_request(request)
{
}

void CreateUndoCommand::undo()
{
    RemoveAuthorCommand removeRequest;
    removeRequest.id = m_result.value();

    RemoveAuthorCommandHandler handler(Repository::Repositories::instance());
    auto result = handler.handle(removeRequest);
    this->setObsolete(result.isError());
}

void CreateUndoCommand::redo()
{
    CreateAuthorCommandHandler handler(Repository::Repositories::instance());
    m_result = handler.handle(m_request);
    this->setObsolete(m_result.isError());
}

Result<QUuid> CreateUndoCommand::result() const
{
    return m_result;
}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

UpdateUndoCommand::UpdateUndoCommand(const UpdateAuthorCommand &request)
    : QUndoCommand(QObject::tr("Create author")), m_request(request)
{
}

void UpdateUndoCommand::undo()
{

    UpdateAuthorCommand command;
    command.req = AutoMapper::AutoMapper::map<UpdateAuthorDTO>(m_oldState.value());

    UpdateAuthorCommandHandler handler(Repository::Repositories::instance());
    auto result = handler.handle(command);
    this->setObsolete(result.isError());
}

void UpdateUndoCommand::redo()
{
    if (m_oldState.isEmpty())
    {
        GetAuthorRequestHandler getHandler(Repository::Repositories::instance());
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

    UpdateAuthorCommandHandler handler(Repository::Repositories::instance());
    m_result = handler.handle(m_request);
    this->setObsolete(m_result.isError());
}

Result<AuthorDTO> UpdateUndoCommand::result() const
{
    return m_result;
}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

RemoveUndoCommand::RemoveUndoCommand(const RemoveAuthorCommand &request)
    : QUndoCommand(QObject::tr("Create author")), m_request(request)
{
}

void RemoveUndoCommand::undo()
{

    CreateAuthorCommand command;
    command.req = AutoMapper::AutoMapper::map<CreateAuthorDTO>(m_result.value());

    CreateAuthorCommandHandler handler(Repository::Repositories::instance());
    handler.handle(command);
    this->setObsolete(m_result.isError());
}

void RemoveUndoCommand::redo()
{
    RemoveAuthorCommandHandler handler(Repository::Repositories::instance());
    m_result = handler.handle(m_request);
    this->setObsolete(m_result.isError());
}

Result<AuthorDTO> RemoveUndoCommand::result() const
{
    return m_result;
}
