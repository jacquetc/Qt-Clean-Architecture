#include "author_undo_commands.h"
#include "cqrs/author/commands/remove_author_command.h"
#include "dto/author/author_dto.h"
#include "features/author/handlers/commands/create_author_command_handler.h"
#include "features/author/handlers/commands/remove_author_command_handler.h"
#include "features/author/handlers/commands/update_author_command_handler.h"
#include "repositories/repositories.h"

using namespace Adapters::Author;
using namespace Application::Features::Author::Commands;
using namespace Contracts::CQRS::Author::Commands;

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
{
}

void UpdateUndoCommand::undo()
{
}

void UpdateUndoCommand::redo()
{
}

Result<AuthorDTO> UpdateUndoCommand::result() const
{
}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

RemoveUndoCommand::RemoveUndoCommand(const RemoveAuthorCommand &request)
{
}

void RemoveUndoCommand::undo()
{
}

void RemoveUndoCommand::redo()
{
}

Result<AuthorDTO> RemoveUndoCommand::result() const
{
}
