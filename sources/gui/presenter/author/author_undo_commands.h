#pragma once

#include "author_signal_bridge.h"
#include "cqrs/author/commands/create_author_command.h"
#include "cqrs/author/commands/remove_author_command.h"
#include "cqrs/author/commands/update_author_command.h"
#include "dto/author/author_dto.h"
#include "features/author/handlers/commands/create_author_command_handler.h"
#include "features/author/handlers/commands/remove_author_command_handler.h"
#include "features/author/handlers/commands/update_author_command_handler.h"
#include "persistence/interface_author_repository.h"
#include "result.h"
#include "undo_redo/undo_redo_command.h"
#include <QObject>

using namespace Contracts::DTO::Author;
using namespace Contracts::CQRS::Author::Commands;
using namespace Contracts::Persistence;
using namespace Application::Features::Author::Commands;
using namespace Presenter::UndoRedo;

namespace Presenter::Author
{

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

class CreateUndoCommand : public UndoRedoCommand
{
  public:
    CreateUndoCommand(Private::AuthorSignalBridge *signal_bridge, QSharedPointer<InterfaceAuthorRepository> repository,
                      const CreateAuthorCommand &request);
    void undo();
    void redo();

  private:
    QSharedPointer<InterfaceAuthorRepository> m_repository;
    CreateAuthorCommand m_request;
    Result<QUuid> m_result;
    Private::AuthorSignalBridge *m_signalBridge;
};
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

class UpdateUndoCommand : public UndoRedoCommand
{
  public:
    UpdateUndoCommand(Private::AuthorSignalBridge *signal_bridge, QSharedPointer<InterfaceAuthorRepository> repository,
                      const UpdateAuthorCommand &request);
    void undo();
    void redo();

  private:
    QSharedPointer<InterfaceAuthorRepository> m_repository;
    UpdateAuthorCommand m_request;
    Result<AuthorDTO> m_result;
    Result<AuthorDTO> m_oldState;
    Private::AuthorSignalBridge *m_signalBridge;
};
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

class RemoveUndoCommand : public UndoRedoCommand
{
  public:
    RemoveUndoCommand(Private::AuthorSignalBridge *signal_bridge, QSharedPointer<InterfaceAuthorRepository> repository,
                      const RemoveAuthorCommand &request);
    void undo();
    void redo();
    Result<AuthorDTO> result() const;

  private:
    QSharedPointer<InterfaceAuthorRepository> m_repository;
    RemoveAuthorCommand m_request;
    Result<AuthorDTO> m_result;
    Private::AuthorSignalBridge *m_signalBridge;
};
} // namespace Presenter::Author
