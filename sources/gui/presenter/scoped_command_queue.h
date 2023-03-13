#include "undo_controller.h"
#include <QCoreApplication>
#include <QDebug>
#include <QHash>
#include <QMetaMethod>
#include <QMutex>
#include <QQueue>
#include <QThread>
#include <QTimer>
#include <QUndoCommand>
#include <QWaitCondition>

namespace Presenter
{

class ScopedCommandQueue : public QObject
{
    Q_OBJECT

  public:
    enum Scope
    {
        Author
    };
    Q_ENUM(Scope)

    ScopedCommandQueue(QObject *parent, UndoController *undoController) : QObject(parent)
    {
        s_undoController = undoController;
        m_timer.setSingleShot(false);
        m_timer.setInterval(10); // adjust the interval as needed
        connect(&m_timer, &QTimer::timeout, this, &ScopedCommandQueue::checkNextCommand);
        m_timer.start();
    }

    static void enqueueCommand(QUndoCommand *command, const Scope &scope)
    {
        QMutexLocker locker(&s_mutex);

        // Add the command to the queue for the given scope
        s_commandQueue[scope].enqueue(command);

        //        // If this is the first command for the given scope, set it as the next command to execute
        //        if (s_commandQueue[scope].size() == 1)
        //        {
        //            checkNextCommand();
        //        }
    }

    static void commandFinished(const Scope &scope)
    {
        QMutexLocker locker(&s_mutex);

        // Remove the finished command from the queue
        QQueue<QUndoCommand *> &queue = s_commandQueue[scope];
        if (!queue.isEmpty())
        {
            queue.dequeue();
        }
        s_currentCommand[scope] = nullptr;

        qDebug() << "a";

        //        // Update the next command for this scope
        //        s_nextCommand[scope] = queue.isEmpty() ? nullptr : queue.head();
    }

    //    static void waitForCommandEnd(QObject *sender, QMetaMethod signal)
    //    {
    //        // Create a mutex to protect the condition variable
    //        QMutex mutex;

    //        // Create a wait condition
    //        QWaitCondition condition;

    //        // Lock the mutex before waiting for the signal
    //        mutex.lock();

    //        // Connect the signal to the slot
    //        QObject::connect(sender, signal, qApp, [&condition]() {
    //            // Wake up the waiting thread
    //            condition.wakeAll();
    //        });

    //        // Wait for the signal to be emitted
    //        condition.wait(&mutex);

    //        // Unlock the mutex after the signal is received
    //        mutex.unlock();
    //    }

  private slots:
    static void checkNextCommand()
    {
        QMutexLocker locker(&s_checkNextCommandMutex);

        // Check if there are any commands waiting to execute
        for (auto it = s_commandQueue.constBegin(); it != s_commandQueue.constEnd(); ++it)
        {
            const Scope &scope = it.key();
            QQueue<QUndoCommand *> &queue = s_commandQueue[scope];
            QUndoCommand *currentCommand = s_currentCommand[scope];

            if (!queue.isEmpty() && !queue.head()->isObsolete() && currentCommand != queue.head())
            {
                qDebug() << "b";
                // s_nextCommand[scope] = queue.isEmpty() ? nullptr : queue.head();
                s_currentCommand[scope] = queue.head();

                // Lock the mutex before accessing the QUndoStack
                QMutexLocker undoLocker(&s_undoStackMutex);

                QTimer::singleShot(0, qApp, [&]() { s_undoController->undoStack()->push(queue.head()); });
            }
        }
    }

  private:
    static QHash<Scope, QQueue<QUndoCommand *>> s_commandQueue;
    static QHash<Scope, QUndoCommand *> s_currentCommand;
    static QMutex s_mutex;
    static QMutex s_checkNextCommandMutex; // Add a mutex for the QUndoStack
    static QMutex s_undoStackMutex;        // Add a mutex for the QUndoStack
    static UndoController *s_undoController;
    QTimer m_timer;
};

} // namespace Presenter
