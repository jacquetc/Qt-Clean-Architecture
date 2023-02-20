#pragma once

#include "features/system/handlers/commands/save_system_as_command_handler.h"
#include "repositories/repositories.h"
#include "result.h"

using namespace Application::Features::System::Commands;

namespace Presenter::System
{

Result<void *> loadSystem(const QUrl &fileName)
{
}

Result<void *> saveSystem()
{
}

Result<void *> saveSystemAs(const SaveSystemAsCommand &request)
{

    SaveSystemAsCommandHandler handler(Repository::Repositories::instance());
    return handler.handle(request);
}

} // namespace Presenter::System
