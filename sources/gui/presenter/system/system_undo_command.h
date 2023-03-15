#pragma once
#include "cqrs/system/commands/load_system_command.h"
#include "infrastructure/skrib/interface_skrib_loader.h"
#include "system_signal_bridge.h"

#include "undo_redo/undo_redo_command.h"

using namespace Contracts::CQRS::System::Commands;
using namespace Presenter::UndoRedo;
using namespace Contracts::Infrastructure::Skrib;

namespace Presenter::System::Private
{

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

class LoadSystemUndoCommand : public UndoRedoCommand
{
  public:
    LoadSystemUndoCommand(SystemSignalBridge *signal_bridge, InterfaceSkribLoader *skribLoader,
                          const LoadSystemCommand &request);
    void undo();
    void redo();

  private:
    InterfaceSkribLoader *m_skribLoader;
    LoadSystemCommand m_request;
    Result<QUuid> m_result;
    SystemSignalBridge *m_signalBridge;
};

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

} // namespace Presenter::System::Private
