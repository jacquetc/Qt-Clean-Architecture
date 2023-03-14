#pragma once

#include "undo_redo_command.h"

namespace Presenter::UndoRedo
{
class QueryCommand : public UndoRedoCommand
{
    Q_OBJECT
  public:
    QueryCommand(const QString &text);

    void setQueryFunction(const std::function<void()> &function);

  private:
    // UndoRedoCommand interface
    void redo() override;
    void undo() override;

  private:
    std::function<void()> m_queryFunction;
};
} // namespace Presenter::UndoRedo
