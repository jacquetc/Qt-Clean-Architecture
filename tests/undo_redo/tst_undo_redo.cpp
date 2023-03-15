#include "undo_redo/query_command.h"
#include "undo_redo/undo_redo_command.h"
#include "undo_redo/undo_redo_system.h"
#include <QtTest>

using namespace Presenter::UndoRedo;

class DummyCommand : public UndoRedoCommand
{

    // UndoRedoCommand interface
  public:
    DummyCommand(const QString &text) : UndoRedoCommand(text)
    {
    }

    void undo() override
    {
    }
    void redo() override
    {
    }
};

class UndoRedoSystemTest : public QObject
{
    Q_OBJECT

  private slots:
    void initTestCase()
    {
    }

    void testInitialState()
    {
        UndoRedoSystem system;

        QVERIFY(!system.canUndo());
        QVERIFY(!system.canRedo());
        QVERIFY(system.undoText().isEmpty());
        QVERIFY(system.redoText().isEmpty());
    }

    void testGeneralCommands()
    {
        UndoRedoSystem system;

        // Push two general commands
        system.push(new DummyCommand("Command 1"), UndoRedoCommand::Author);
        system.push(new DummyCommand("Command 2"), UndoRedoCommand::Author);

        QTRY_VERIFY_WITH_TIMEOUT(system.canUndo(), 500);
        QTRY_VERIFY_WITH_TIMEOUT(!system.canRedo(), 500);
        QCOMPARE(system.undoText(), QString("Command 2"));
        QCOMPARE(system.redoText(), QString());

        // Undo the first command
        system.undo();

        QTRY_VERIFY_WITH_TIMEOUT(system.canUndo(), 500);
        QTRY_VERIFY_WITH_TIMEOUT(system.canRedo(), 500);
        QCOMPARE(system.undoText(), QString("Command 1"));
        QCOMPARE(system.redoText(), QString("Command 2"));

        // Redo the first command
        system.redo();

        QTRY_VERIFY_WITH_TIMEOUT(system.canUndo(), 500);
        QVERIFY(!system.canRedo());
        QCOMPARE(system.undoText(), QString("Command 2"));
        QCOMPARE(system.redoText(), QString());

        // Push a third general command
        system.push(new DummyCommand("Command 3"), UndoRedoCommand::Author);

        QTRY_VERIFY_WITH_TIMEOUT(system.canUndo(), 500);
        QVERIFY(!system.canRedo());
        QCOMPARE(system.undoText(), QString("Command 3"));
        QCOMPARE(system.redoText(), QString());

        // Undo all commands
        system.undo();
        system.undo();
        system.undo();

        QTRY_VERIFY_WITH_TIMEOUT(!system.canUndo(), 500);
        QTRY_VERIFY_WITH_TIMEOUT(system.canRedo(), 500);
        QCOMPARE(system.undoText(), QString());
        QCOMPARE(system.redoText(), QString("Command 1"));

        // Redo all commands
        system.redo();
        system.redo();
        system.redo();

        QTRY_VERIFY_WITH_TIMEOUT(system.canUndo(), 500);
        QTRY_VERIFY_WITH_TIMEOUT(!system.canRedo(), 500);
        QCOMPARE(system.undoText(), QString("Command 3"));
        QCOMPARE(system.redoText(), QString());
    }
};
QTEST_GUILESS_MAIN(UndoRedoSystemTest)

#include "tst_undo_redo.moc"
