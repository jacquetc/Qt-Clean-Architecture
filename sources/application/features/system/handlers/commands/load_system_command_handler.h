#pragma once

#include "application_global.h"
#include "cqrs/system/commands/load_system_command.h"
#include "handler.h"
#include "persistence/interface_repositories.h"
#include "result.h"

using namespace Contracts::DTO::System;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::System::Commands;

namespace Application::Features::System::Commands
{
class SKRAPPLICATIONEXPORT LoadSystemCommandHandler : public Handler
{
  public:
    LoadSystemCommandHandler(InterfaceRepositories *repositories);
    Result<void *> handle(const LoadSystemCommand &request);

  private:
    InterfaceRepositories *m_repositories;
};
} // namespace Application::Features::System::Commands
