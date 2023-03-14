#include "undo_redo_command.h"
#include "QtConcurrent/qtconcurrentrun.h"

#include <QFuture>
#include <QFutureWatcher>

using namespace Presenter::UndoRedo;

UndoRedoCommand::UndoRedoCommand(const QString &text)
    : QObject(nullptr), m_text(text), m_running(false), m_finished(false)
{
}

void UndoRedoCommand::asyncUndo()
{
    m_running = true;
    QFuture<void> future = QtConcurrent::run(&UndoRedoCommand::undo, this);
    QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);
    connect(watcher, &QFutureWatcher<void>::finished, this, &UndoRedoCommand::onFinished);
    watcher->setFuture(future);
}

void UndoRedoCommand::asyncRedo()
{
    m_running = true;
    QFuture<void> future = QtConcurrent::run(&UndoRedoCommand::redo, this);
    QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);
    connect(watcher, &QFutureWatcher<void>::finished, this, &UndoRedoCommand::onFinished);
    watcher->setFuture(future);
}

void UndoRedoCommand::onFinished()
{
    m_running = false;
    m_finished = true;
    emit finished();
}

bool UndoRedoCommand::obsolete() const
{
    return m_obsolete;
}

void UndoRedoCommand::setObsolete(bool newObsolete)
{
    m_obsolete = newObsolete;
}

QString UndoRedoCommand::text() const
{
    return m_text;
}

void UndoRedoCommand::setText(const QString &newText)
{
    m_text = newText;
}

UndoRedoCommand::Scope UndoRedoCommand::scope() const
{
    return m_scope;
}

void UndoRedoCommand::setScope(Scope newScope)
{
    m_scope = newScope;
}
