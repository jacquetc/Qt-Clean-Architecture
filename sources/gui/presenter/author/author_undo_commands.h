#pragma once

#include "author_signal_bridge.h"
#include "cqrs/author/commands/create_author_command.h"
#include "cqrs/author/commands/remove_author_command.h"
#include "cqrs/author/commands/update_author_command.h"
#include "dto/author/author_dto.h"
#include "persistence/interface_author_repository.h"
#include "result.h"
#include "undo_redo/undo_redo_command.h"
#include <QObject>

using namespace Contracts::DTO::Author;
using namespace Contracts::CQRS::Author::Commands;
using namespace Contracts::Persistence;
using namespace Presenter::UndoRedo;

namespace Presenter::Author::Private
{

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

class CreateAuthorUndoCommand : public UndoRedoCommand
{
  public:
    CreateAuthorUndoCommand(Private::AuthorSignalBridge *signal_bridge,
                            QSharedPointer<InterfaceAuthorRepository> repository, const CreateAuthorCommand &request);
    Result<void> undo();
    Result<void> redo();

  private:
    QSharedPointer<InterfaceAuthorRepository> m_repository;
    CreateAuthorCommand m_request;
    Result<QUuid> m_result;
    Private::AuthorSignalBridge *m_signalBridge;
};
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

class UpdateAuthorUndoCommand : public UndoRedoCommand
{
  public:
    UpdateAuthorUndoCommand(Private::AuthorSignalBridge *signal_bridge,
                            QSharedPointer<InterfaceAuthorRepository> repository, const UpdateAuthorCommand &request);
    Result<void> undo();
    Result<void> redo();

  private:
    QSharedPointer<InterfaceAuthorRepository> m_repository;
    UpdateAuthorCommand m_request;
    Result<AuthorDTO> m_result;
    Result<AuthorDTO> m_oldState;
    Private::AuthorSignalBridge *m_signalBridge;
};
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

class RemoveAuthorUndoCommand : public UndoRedoCommand
{
  public:
    RemoveAuthorUndoCommand(Private::AuthorSignalBridge *signal_bridge,
                            QSharedPointer<InterfaceAuthorRepository> repository, const RemoveAuthorCommand &request);
    Result<void> undo();
    Result<void> redo();
    Result<AuthorDTO> result() const;

  private:
    QSharedPointer<InterfaceAuthorRepository> m_repository;
    RemoveAuthorCommand m_request;
    Result<AuthorDTO> m_result;
    Private::AuthorSignalBridge *m_signalBridge;
};
} // namespace Presenter::Author::Private
