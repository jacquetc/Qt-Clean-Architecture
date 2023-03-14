/**
 * @file create_author_command_handler.h
 * @brief Contains the definition of the CreateAuthorCommandHandler class.
 */

#pragma once

#include "application_global.h"
#include "cqrs/author/commands/create_author_command.h"
#include "handler.h"
#include "persistence/interface_author_repository.h"
#include "result.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Commands;

namespace Application::Features::Author::Commands
{
/**
 * @brief Handles the CreateAuthorCommand and persists the new author to the data store.
 */
class SKRAPPLICATIONEXPORT CreateAuthorCommandHandler : public Handler
{
    Q_OBJECT
  public:
    /**
     * @brief Constructs a new CreateAuthorCommandHandler object.
     * @param repositories A pointer to the interface repositories object.
     */
    CreateAuthorCommandHandler(QSharedPointer<InterfaceAuthorRepository> repository);

    /**
     * @brief Handles a CreateAuthorCommand object and returns the UUID of the newly created author.
     * @param request The CreateAuthorCommand object to handle.
     * @return A Result object containing the UUID of the newly created author, or an error message if the operation
     * failed.
     */
    Result<QUuid> handle(const CreateAuthorCommand &request);

  private:
    QSharedPointer<InterfaceAuthorRepository> m_repository; // A pointer to the interface repositories object.
    Result<QUuid> handleImpl(const CreateAuthorCommand &request);
};
} // namespace Application::Features::Author::Commands
