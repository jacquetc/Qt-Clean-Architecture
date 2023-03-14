#pragma once

#include "undo_redo_command.h"
#include <QHash>
#include <QObject>
#include <QQueue>
#include <QSharedPointer>

namespace Presenter::UndoRedo
{
class UndoRedoSystem : public QObject
{
    Q_OBJECT
  public:
    UndoRedoSystem(QObject *parent = nullptr);

    Q_INVOKABLE bool canUndo() const;

    Q_INVOKABLE bool canRedo() const;

    Q_INVOKABLE void undo();

    Q_INVOKABLE void redo();

    Q_INVOKABLE void push(Presenter::UndoRedo::UndoRedoCommand *command,
                          const Presenter::UndoRedo::UndoRedoCommand::Scope &scope);

  private slots:
    void onCommandFinished();

  signals:
    void stateChanged();

  private:
    void executeNextCommand(const UndoRedoCommand::Scope &scope);

    int m_currentIndex;
    QQueue<QSharedPointer<UndoRedoCommand>> m_generalCommandQueue;
    QHash<UndoRedoCommand::Scope, QQueue<QSharedPointer<UndoRedoCommand>>> m_scopedCommandQueueHash;
    QHash<UndoRedoCommand::Scope, QSharedPointer<UndoRedoCommand>> m_currentCommandHash;
};
} // namespace Presenter::UndoRedo
