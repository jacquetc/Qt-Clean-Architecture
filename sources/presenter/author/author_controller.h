#pragma once

#include "author_undo_commands.h"
#include "cqrs/author/commands/create_author_command.h"
#include "cqrs/author/commands/update_author_command.h"
#include "dto/author/author_dto.h"
#include "dto/author/update_author_dto.h"
#include "features/author/handlers/queries/get_author_list_request_handler.h"
#include "features/author/handlers/queries/get_author_request_handler.h"
#include "presenter_global.h"
#include "repositories/repositories.h"
#include "result.h"
#include "undo_controller.h"

using namespace Contracts::DTO::Author;
using namespace Application::Features::Author::Queries;
using namespace Contracts::CQRS::Author::Commands;

namespace Presenter::Author
{

/**
 * @brief Returns the AuthorDTO for the specified UUID.
 *
 * @param uuid The UUID of the author to retrieve.
 * @return The AuthorDTO for the specified UUID.
 */
Result<AuthorDTO> SKRPRESENTEREXPORT get(const QUuid &uuid)
{
    GetAuthorRequest request;
    request.id = uuid;

    GetAuthorRequestHandler handler(Repository::Repositories::instance());
    return handler.handle(request);
}

//-----------------------------------------------------------------

/**
 * @brief Returns a list of all AuthorDTOs.
 *
 * @return A list of all AuthorDTOs.
 */
SKRPRESENTEREXPORT Result<QList<AuthorDTO>> getAll()
{
    GetAuthorListRequestHandler handler(Repository::Repositories::instance());
    return handler.handle();
}

//-----------------------------------------------------------------
/**
 * @brief Creates a new author with the specified DTO.
 *
 * @param dto The DTO representing the author to create.
 * @return The UUID of the created author.
 */
SKRPRESENTEREXPORT Result<QUuid> create(const CreateAuthorDTO &dto)
{
    CreateAuthorCommand request;
    request.req = dto;

    auto command = new CreateUndoCommand(request);
    UndoController::instance()->undoStack()->push(command);
    return command->result();
}

//-----------------------------------------------------------------
/**
 * @brief Updates the author with the specified DTO.
 *
 * @param dto The DTO representing the author to update.
 * @return The updated AuthorDTO.
 */
SKRPRESENTEREXPORT Result<AuthorDTO> update(const UpdateAuthorDTO &dto)
{
    UpdateAuthorCommand request;
    request.req = dto;
    auto command = new UpdateUndoCommand(request);
    UndoController::instance()->undoStack()->push(command);
    return command->result();
}

//-----------------------------------------------------------------
/**
 * @brief Removes the author with the specified UUID.
 *
 * @param uuid The UUID of the author to remove.
 * @return The AuthorDTO for the removed author.
 */
SKRPRESENTEREXPORT Result<AuthorDTO> remove(const QUuid &uuid)
{
    RemoveAuthorCommand request;
    request.id = uuid;
    auto command = new RemoveUndoCommand(request);
    UndoController::instance()->undoStack()->push(command);
    return command->result();
}

} // namespace Presenter::Author
