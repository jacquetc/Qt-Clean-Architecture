#pragma once

#include "cqrs/system/commands/save_system_as_command.h"
#include "handler.h"
#include "persistence/interface_repositories.h"
#include "result.h"

using namespace Contracts::DTO::System;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::System::Commands;

namespace Application::Features::System::Commands
{
class SaveSystemAsCommandHandler : public Handler
{
  public:
    SaveSystemAsCommandHandler(InterfaceRepositories *repositories);
    Result<void *> handle(const SaveSystemAsCommand &request);

  private:
    InterfaceRepositories *m_repositories;
};
} // namespace Application::Features::System::Commands
