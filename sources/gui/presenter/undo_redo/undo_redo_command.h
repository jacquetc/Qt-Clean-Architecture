#pragma once

#include <QObject>

namespace Presenter::UndoRedo
{
class UndoRedoCommand : public QObject
{
    Q_OBJECT
  public:
    enum Scope
    {
        Author
    };
    Q_ENUM(Scope)

    UndoRedoCommand(const QString &text);

    virtual void undo() = 0;
    virtual void redo() = 0;

    void asyncUndo();

    void asyncRedo();

    bool isRunning() const
    {
        return m_running;
    }

    UndoRedoCommand::Scope scope() const;
    void setScope(UndoRedoCommand::Scope newScope);

    QString text() const;
    void setText(const QString &newText);

    bool obsolete() const;
    void setObsolete(bool newObsolete);

  signals:
    void finished();

  private slots:
    void onFinished();

  private:
    bool m_obsolete;
    bool m_finished;
    bool m_running;
    QString m_text;
    UndoRedoCommand::Scope m_scope;
};
} // namespace Presenter::UndoRedo
