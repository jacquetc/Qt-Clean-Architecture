#include "system_undo_command.h"
#include "features/system/handlers/commands/load_system_command_handler.h"

using namespace Presenter::System::Private;
using namespace Application::Features::System::Commands;
using namespace Contracts::Infrastructure::Skrib;

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

LoadSystemUndoCommand::LoadSystemUndoCommand(SystemSignalBridge *signalBridge, InterfaceSkribLoader *skribLoader,
                                             const LoadSystemCommand &request)
    : UndoRedoCommand(QObject::tr("Load system")), m_signalBridge(signalBridge), m_skribLoader(skribLoader),
      m_request(request)
{
}

Result<void> LoadSystemUndoCommand::undo()
{
    Q_UNREACHABLE();
}

Result<void> LoadSystemUndoCommand::redo()
{
    LoadSystemCommandHandler handler(m_skribLoader);
    auto result = handler.handle(m_request);
    if (result.isSuccess())
    {
        emit m_signalBridge->systemLoaded(result);
    }
    return Result<void>(result.error());
}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
