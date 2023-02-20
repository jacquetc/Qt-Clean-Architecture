#ifndef AUTHOR_UNDO_COMMANDS_H
#define AUTHOR_UNDO_COMMANDS_H

#include "cqrs/author/commands/create_author_command.h"
#include "cqrs/author/commands/remove_author_command.h"
#include "cqrs/author/commands/update_author_command.h"
#include "dto/author/author_dto.h"
#include "result.h"
#include <QObject>
#include <QUndoStack>

using namespace Contracts::DTO::Author;
using namespace Contracts::CQRS::Author::Commands;

namespace Adapters::Author
{

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

class CreateUndoCommand : public QUndoCommand
{
  public:
    CreateUndoCommand(const CreateAuthorCommand &request);
    void undo();
    void redo();
    Result<QUuid> result() const;

  private:
    CreateAuthorCommand m_request;
    Result<QUuid> m_result;
};
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

class UpdateUndoCommand : public QUndoCommand
{
  public:
    UpdateUndoCommand(const UpdateAuthorCommand &request);
    void undo();
    void redo();
    Result<AuthorDTO> result() const;

  private:
    UpdateAuthorCommand m_request;
    Result<AuthorDTO> m_result;
};
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

class RemoveUndoCommand : public QUndoCommand
{
  public:
    RemoveUndoCommand(const RemoveAuthorCommand &request);
    void undo();
    void redo();
    Result<AuthorDTO> result() const;

  private:
    CreateAuthorCommand m_request;
    Result<AuthorDTO> m_result;
};
} // namespace Adapters::Author

#endif // AUTHORUNDOCOMMANDS_H
