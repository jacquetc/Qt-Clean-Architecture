#pragma once

#include "application_global.h"
#include "cqrs/system/commands/load_system_command.h"
#include "handler.h"
#include "result.h"

using namespace Contracts::DTO::System;
using namespace Contracts::CQRS::System::Commands;

namespace Application::Features::System::Commands
{
class SKRAPPLICATIONEXPORT LoadSystemCommandHandler : public Handler
{
  public:
    LoadSystemCommandHandler();
    void handle(const LoadSystemCommand &request);

  private:
  signals:
    void systemLoaded(Result<void *>);
};
} // namespace Application::Features::System::Commands
