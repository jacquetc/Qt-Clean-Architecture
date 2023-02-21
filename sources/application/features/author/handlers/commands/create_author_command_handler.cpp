#include "create_author_command_handler.h"
#include "QtConcurrent/qtconcurrenttask.h"
#include "automapper/automapper.h"
#include "cqrs/author/validators/create_author_command_validator.h"
#include "persistence/interface_author_repository.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Validators;
using namespace Application::Features::Author::Commands;

CreateAuthorCommandHandler::CreateAuthorCommandHandler(InterfaceRepositories *repositories)
    : Handler(), m_repositories(repositories)
{
}

Result<QUuid> CreateAuthorCommandHandler::handle(const CreateAuthorCommand &request)
{
    // Execute a task asynchronously in a separate thread, and return the result as a future
    return waitInEventLoop<QUuid>(
        QtConcurrent::task([this](CreateAuthorCommand request) {
            // Get the author repository from the repository manager
            InterfaceAuthorRepository *repository =
                dynamic_cast<InterfaceAuthorRepository *>(m_repositories->get(InterfaceRepositories::Entities::Author));

            // Validate the create author command using the validator
            auto validator = CreateAuthorCommandValidator(repository);
            Result<void *> validatorResult = validator.validate(request.req);
            if (validatorResult.hasError())
            {
                // Return the validation error as a Result object
                return Result<QUuid>(validatorResult.error());
            }

            // Map the create author command to a domain author object and generate a UUID
            auto author = AutoMapper::AutoMapper::map<Domain::Author>(request.req);
            author.setUuid(QUuid::createUuid());

            // Set the creation and update timestamps to the current date and time
            author.setCreationDate(QDateTime::currentDateTime());
            author.setUpdateDate(QDateTime::currentDateTime());

            // Add the author to the repository
            auto authorResult = repository->add(std::move(author));
            if (authorResult.hasError())
            {
                // Return the repository error as a Result object
                return Result<QUuid>(authorResult.error());
            }

            // Return the UUID of the newly created author as a Result object
            return Result<QUuid>(authorResult.value().uuid());
        })
            .withArguments(request) // Pass the request object as an argument to the task function
            .spawn());              // Start the task and return the future as a Result object
}
