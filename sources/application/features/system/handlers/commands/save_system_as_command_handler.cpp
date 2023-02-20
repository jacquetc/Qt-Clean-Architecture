#include "save_system_as_command_handler.h"
#include "cqrs/system/validators/save_system_as_command_validator.h"
#include "database/skrib/skrib_file.h"
#include "database/skrib/skrib_file_context.h"
#include "persistence/interface_author_repository.h"

using namespace Application::Features::System::Commands;
using namespace Contracts::CQRS::Author::Validators;

SaveSystemAsCommandHandler::SaveSystemAsCommandHandler(InterfaceRepositories *repositories)
    : Handler(), m_repositories(repositories)
{
}

Result<void *> SaveSystemAsCommandHandler::handle(const SaveSystemAsCommand &request)
{

    // validate:
    auto validator = SaveSystemAsCommandValidator();
    Result<void *> validatorResult = validator.validate(request.req);
    if (validatorResult.hasError())
    {
        return Result<void *>(validatorResult.error());
    }

    SkribFileContext *context = new SkribFileContext(request.req.fileName());
    auto initResult = context->init();
    if (initResult.hasError())
    {
        return Result<void *>(initResult.error());
    }

    // get repository
    InterfaceAuthorRepository *repository =
        dynamic_cast<InterfaceAuthorRepository *>(m_repositories->get(InterfaceRepositories::Entities::Author));

    repository->setDatabase(new Database::Skrib::SkribFile<Domain::Author>(context));

    return Result<void *>();
}
