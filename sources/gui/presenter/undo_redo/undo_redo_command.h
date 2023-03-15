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
        Author,
        All
    };
    Q_ENUM(Scope)

    UndoRedoCommand(const QString &text);

    virtual void undo() = 0;

    virtual void redo() = 0;

    void asyncUndo();

    void asyncRedo();

    bool isRunning() const;

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
    bool m_obsolete;                /*!< A boolean representing the obsolete state of the command. */
    bool m_finished;                /*!< A boolean representing the finished state of the command. */
    bool m_running;                 /*!< A boolean representing the running state of the command. */
    QString m_text;                 /*!< A QString representing the text description of the command. */
    UndoRedoCommand::Scope m_scope; /*!< The command's scope as an UndoRedoCommand::Scope enumeration value. */
};
} // namespace Presenter::UndoRedo
