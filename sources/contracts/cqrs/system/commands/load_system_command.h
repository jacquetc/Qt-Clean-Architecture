#pragma once

#include "contracts_global.h"
#include "dto/system/load_system_dto.h"

namespace Contracts::CQRS::System::Commands
{
class SKRCONTRACTSEXPORT LoadSystemCommand
{
  public:
    LoadSystemCommand()
    {
    }

    Contracts::DTO::System::LoadSystemDTO req;
};
} // namespace Contracts::CQRS::System::Commands
