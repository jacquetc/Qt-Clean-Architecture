#ifndef AUTHOR_CONTROLLER_H
#define AUTHOR_CONTROLLER_H

#include "author_undo_commands.h"
#include "cqrs/author/commands/create_author_command.h"
#include "cqrs/author/commands/update_author_command.h"
#include "dto/author/author_dto.h"
#include "dto/author/update_author_dto.h"
#include "features/author/handlers/queries/get_author_request_handler.h"
#include "repositories/repositories.h"
#include "result.h"
#include "undo_controller.h"
#include <QObject>

using namespace Contracts::DTO::Author;
using namespace Application::Features::Author::Queries;
using namespace Contracts::CQRS::Author::Commands;

namespace Adapters::Author
{

Result<AuthorDTO> get(const QUuid &uuid)
{
    GetAuthorRequest request;
    request.id = uuid;

    GetAuthorRequestHandler handler(Repository::Repositories::instance());
    return handler.handle(request);
}

//-----------------------------------------------------------------

Result<QUuid> create(const CreateAuthorDTO &dto)
{
    CreateAuthorCommand request;
    request.req = dto;

    auto command = new CreateUndoCommand(request);
    UndoController::instance()->undoStack()->push(command);
    return command->result();
}

//-----------------------------------------------------------------

Result<AuthorDTO> update(const UpdateAuthorDTO &dto)
{
    UpdateAuthorCommand request;
    request.req = dto;
    auto command = new UpdateUndoCommand(request);
    UndoController::instance()->undoStack()->push(command);
    return command->result();
}

} // namespace Adapters::Author
#endif // AUTHOR_CONTROLLER_H
