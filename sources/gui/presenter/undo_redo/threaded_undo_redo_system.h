#pragma once

#include "undo_redo_system.h"
#include <QMutex>
#include <QObject>
#include <QThread>

namespace Presenter::UndoRedo
{
class ThreadedUndoRedoSystem : public QObject
{
    Q_OBJECT
  public:
    ThreadedUndoRedoSystem(QObject *parent = nullptr);

    ~ThreadedUndoRedoSystem();

    static ThreadedUndoRedoSystem *instance();

    bool canUndo() const;

    bool canRedo() const;

    void undo();

    void redo();

    void push(UndoRedoCommand *command, const UndoRedoCommand::Scope &scope);

  signals:
    void stateChanged();

  private slots:
    void startUndoRedoSystem();

    void onUndoRedoSystemStateChanged();

  private:
    static ThreadedUndoRedoSystem *m_instance;
    mutable QMutex m_mutex;
    UndoRedoSystem *m_undoRedoSystem = nullptr;
    QThread *m_thread = nullptr;
};
} // namespace Presenter::UndoRedo
