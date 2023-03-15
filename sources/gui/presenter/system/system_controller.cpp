#include "system_controller.h"
#include "cqrs/system/commands/load_system_command.h"
#include "skrib/skrib_loader.h"
#include "system/system_undo_command.h"

using namespace Presenter::System;
using namespace Presenter::System::Private;
using namespace Presenter::UndoRedo;
using namespace Contracts::CQRS::System::Commands;
using namespace Infrastructure::Skrib;

QScopedPointer<SystemController> SystemController::s_instance = QScopedPointer<SystemController>(nullptr);
SystemSignalBridge *SystemController::s_signal_bridge;
InterfaceRepositoryProvider *SystemController::s_repositoryProvider;
ThreadedUndoRedoSystem *SystemController::s_undo_redo_system;

SystemController::SystemController(InterfaceRepositoryProvider *repositoryProvider) : QObject(nullptr)
{
    s_repositoryProvider = repositoryProvider;
    s_signal_bridge = new SystemSignalBridge(this);
    // connections for undo commands:

    connect(s_signal_bridge, &SystemSignalBridge::systemLoaded, this, &SystemController::systemLoaded);
    connect(s_signal_bridge, &SystemSignalBridge::systemSaved, this, &SystemController::systemSaved);

    s_undo_redo_system = ThreadedUndoRedoSystem::instance();

    s_instance.reset(this);
}

SystemController *SystemController::instance()
{
    return s_instance.data();
}

void System::SystemController::loadSystem(const LoadSystemDTO &dto)
{
    LoadSystemCommand request;
    request.req = dto;
    auto *skribLoader = new SkribLoader(s_repositoryProvider);
    auto command = new LoadSystemUndoCommand(s_signal_bridge, skribLoader, request);
    s_undo_redo_system->push(command, UndoRedoCommand::Scope::All);
    s_undo_redo_system->clear();
}

void System::SystemController::saveSystem()
{
    Q_UNIMPLEMENTED();
}

void System::SystemController::saveSystemAs(const SaveSystemAsDTO &dto)
{
    Q_UNIMPLEMENTED();
    //    SaveSystemAsCommand request;
    //    request.req = dto;
}
