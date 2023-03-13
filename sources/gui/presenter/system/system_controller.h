#pragma once

#include "features/system/handlers/commands/load_system_command_handler.h"
#include "features/system/handlers/commands/save_system_as_command_handler.h"
#include "presenter_global.h"
#include "result.h"

using namespace Application::Features::System::Commands;

namespace Presenter::System
{

SKRPRESENTEREXPORT Result<void *> loadSystem(const LoadSystemDTO &dto)
{
    LoadSystemCommand command;
    command.req = dto;
    //    LoadSystemCommandHandler handler(Repository::Repositories::instance());
    //    return handler.handle(command);
}

SKRPRESENTEREXPORT Result<void *> saveSystem()
{
    Q_UNIMPLEMENTED();
}

SKRPRESENTEREXPORT Result<void *> saveSystemAs(const SaveSystemAsDTO &dto)
{
    Q_UNIMPLEMENTED();
    SaveSystemAsCommand command;
    command.req = dto;

    //    SaveSystemAsCommandHandler handler(Repository::Repositories::instance());
    //    return handler.handle(command);
}

} // namespace Presenter::System
