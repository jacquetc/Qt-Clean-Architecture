#include "load_system_command_handler.h"
#include "cqrs/system/validators/load_system_command_validator.h"

using namespace Application::Features::System::Commands;
using namespace Contracts::CQRS::Author::Validators;

LoadSystemCommandHandler::LoadSystemCommandHandler() : Handler()
{
}

void LoadSystemCommandHandler::handle(const LoadSystemCommand &request)
{

    // validate:
    auto validator = LoadSystemCommandValidator();
    Result<void> validatorResult = validator.validate(request.req);
    if (validatorResult.hasError())
    {
        // return Result<void>(validatorResult.error());
    }

    // return Result<void>();
}
