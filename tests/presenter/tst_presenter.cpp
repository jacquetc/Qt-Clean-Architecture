#include "QtConcurrent/qtconcurrentrun.h"
#include "author/author_controller.h"
#include "automapper/automapper.h"
#include "dto/author/author_dto.h"
#include "dummy_author_repository.h"
#include "dummy_repository_provider.h"
#include "scoped_command_queue.h"
#include "undo_controller.h"
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QHash>
#include <QList>
#include <QTime>
#include <QVariant>
#include <QtTest>

using namespace Presenter::Author;
using namespace Contracts::DTO::Author;

class PresenterTest : public QObject
{
    Q_OBJECT

  public:
    PresenterTest();
    ~PresenterTest();

  public slots:

  private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void addAuthor();
    void addAuthorAsync();
    void addAuthorAsync_TwoRapid();

  private:
    DummyRepositoryProvider *m_repositoryProvider;
    QSharedPointer<DummyAuthorRepository> m_repository;
    AuthorController *m_authorController;
};

PresenterTest::PresenterTest()
{
    m_repositoryProvider = new DummyRepositoryProvider(this);
}

PresenterTest::~PresenterTest()
{
}

void PresenterTest::initTestCase()
{
    m_repository.reset(new DummyAuthorRepository(this));
    m_repositoryProvider->registerRepository(DummyRepositoryProvider::Author, m_repository);
    new ScopedCommandQueue(this, UndoController::instance());

    m_authorController = new AuthorController(m_repositoryProvider);
}

void PresenterTest::cleanupTestCase()
{
}

void PresenterTest::init()
{
}

void PresenterTest::cleanup()
{
}

// ----------------------------------------------------------

void PresenterTest::addAuthor()
{

    // Create an AuthorDTO to add
    CreateAuthorDTO dto;
    dto.setName("new author");
    dto.setRelative(QUuid::createUuid());

    // prefill the dummy repo:
    auto author = AutoMapper::AutoMapper::map<Domain::Author>(dto);
    author.setUuid(QUuid::createUuid());
    m_repository->fillAdd(author);

    // invoke

    QSignalSpy spy(m_authorController, &AuthorController::authorCreated);
    QVERIFY(spy.isValid());

    // Call the create method and wait for it to finish
    QFuture<Result<QUuid>> future = QtConcurrent::run([&]() { return m_authorController->create(dto); });
    QTRY_COMPARE_WITH_TIMEOUT(future.isFinished(), true, 5000);

    // Check that the result is successful and that a valid UUID was returned
    Result<QUuid> result = future.result();
    QVERIFY(result.isSuccess());
    QVERIFY(!result.value().isNull());

    if (!result)
    {
        qDebug() << result.error().message() << result.error().data();
    }
    QVERIFY(result.isSuccess());

    QVERIFY(spy.wait(5000));
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Result<QUuid> signalResult = arguments.at(0).value<Result<QUuid>>();
    if (!signalResult)
    {
        qDebug() << signalResult.error().message() << signalResult.error().data();
    }

    QVERIFY(signalResult.isSuccess());
}
// ----------------------------------------------------------

void PresenterTest::addAuthorAsync()
{
    // Create an AuthorDTO to add
    CreateAuthorDTO dto;
    dto.setName("new author");
    dto.setRelative(QUuid::createUuid());

    // prefill the dummy repo:
    auto author = AutoMapper::AutoMapper::map<Domain::Author>(dto);
    author.setUuid(QUuid::createUuid());
    m_repository->fillAdd(author);

    // invoke

    QSignalSpy spy(m_authorController, &AuthorController::authorCreated);
    QVERIFY(spy.isValid());

    m_authorController->createAsync(dto);

    QVERIFY(spy.wait(5000));
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Result<QUuid> signalResult = arguments.at(0).value<Result<QUuid>>();
    if (!signalResult)
    {
        qDebug() << signalResult.error().message() << signalResult.error().data();
    }

    QVERIFY(signalResult.isSuccess());
}
// ----------------------------------------------------------

void PresenterTest::addAuthorAsync_TwoRapid()
{
    // Create an AuthorDTO to add
    CreateAuthorDTO dto;
    dto.setName("new author");
    dto.setRelative(QUuid::createUuid());

    // prefill the dummy repo:
    auto author = AutoMapper::AutoMapper::map<Domain::Author>(dto);
    author.setUuid(QUuid::createUuid());
    m_repository->fillAdd(author);

    // invoke

    QSignalSpy spy(m_authorController, &AuthorController::authorCreated);
    QVERIFY(spy.isValid());

    m_authorController->createAsync(dto);
    m_authorController->createAsync(dto);

    QVERIFY(spy.wait(5000));
    QCOMPARE(spy.count(), 1);
    QVERIFY(spy.wait(5000));
    QCOMPARE(spy.count(), 2);
    QList<QVariant> arguments = spy.takeFirst();
    Result<QUuid> signalResult = arguments.at(0).value<Result<QUuid>>();
    if (!signalResult)
    {
        qDebug() << signalResult.error().message() << signalResult.error().data();
    }

    QVERIFY(signalResult.isSuccess());
}

QTEST_GUILESS_MAIN(PresenterTest)

#include "tst_presenter.moc"
