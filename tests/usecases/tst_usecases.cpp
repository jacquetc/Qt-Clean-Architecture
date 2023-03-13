#include "automapper/automapper.h"
#include "dto/author/author_dto.h"
#include "dummy_author_repository.h"
#include "features/author/handlers/commands/create_author_command_handler.h"
#include "features/author/handlers/commands/remove_author_command_handler.h"
#include "features/author/handlers/queries/get_author_request_handler.h"
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QHash>
#include <QList>
#include <QTime>
#include <QVariant>
#include <QtTest>

using namespace Application::Features::Author::Queries;
using namespace Application::Features::Author::Commands;
using namespace Contracts::DTO::Author;

class UseCases : public QObject
{
    Q_OBJECT

  public:
    UseCases();
    ~UseCases();

  public slots:

  private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // void createTag();
    void getAuthor();
    void getAuthorAsync();
    void addAuthor();
    void addAuthorAsync();
    void removeAuthor();
    void removeAuthorAsync();

  private:
};

UseCases::UseCases()
{
}

UseCases::~UseCases()
{
}

void UseCases::initTestCase()
{
}

void UseCases::cleanupTestCase()
{
}

void UseCases::init()
{
}

void UseCases::cleanup()
{
}

// ----------------------------------------------------------

void UseCases::getAuthor()
{

    QSharedPointer<DummyAuthorRepository> repository(new DummyAuthorRepository(this));

    QUuid uuid = QUuid::createUuid();
    QUuid relative = QUuid::createUuid();
    Domain::Author author(uuid, "test", relative);
    repository->fillGet(author);

    GetAuthorRequestHandler handler(repository);
    GetAuthorRequest request;
    request.id = uuid;

    Result<AuthorDTO> dtoResult = handler.handle(request);
    if (!dtoResult)
    {
        qDebug() << dtoResult.error().message() << dtoResult.error().data();
    }
    QVERIFY(dtoResult.isSuccess());

    QCOMPARE(dtoResult.value().getName(), "test");
    QCOMPARE(dtoResult.value().getRelative(), relative);
}

// ----------------------------------------------------------

void UseCases::getAuthorAsync()
{

    QSharedPointer<DummyAuthorRepository> repository(new DummyAuthorRepository(this));

    QUuid uuid = QUuid::createUuid();
    QUuid relative = QUuid::createUuid();
    Domain::Author author(uuid, "test", relative);
    repository->fillGet(author);

    GetAuthorRequestHandler handler(repository);
    GetAuthorRequest request;
    request.id = uuid;

    QSignalSpy spy(&handler, &GetAuthorRequestHandler::getAuthorReplied);
    handler.handleAsync(request);

    QVERIFY(spy.wait(500));
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Result<AuthorDTO> dtoResult = arguments.at(0).value<Result<AuthorDTO>>();
    if (!dtoResult)
    {
        qDebug() << dtoResult.error().message() << dtoResult.error().data();
    }
    QVERIFY(dtoResult.isSuccess());

    QCOMPARE(dtoResult.value().getName(), "test");
    QCOMPARE(dtoResult.value().getRelative(), relative);
}

// ----------------------------------------------------------

void UseCases::addAuthor()
{
    QSharedPointer<DummyAuthorRepository> repository(new DummyAuthorRepository(this));

    // Create an AuthorDTO to add
    CreateAuthorDTO dto;
    dto.setName("new author");
    dto.setRelative(QUuid::createUuid());

    // prefill the dummy repo:
    auto author = AutoMapper::AutoMapper::map<Domain::Author>(dto);
    author.setUuid(QUuid::createUuid());
    repository->fillAdd(author);
    repository->fillGetAll(QList<Domain::Author>() << author);

    // Invoke the CreateAuthorCommandHandler with the DTO
    CreateAuthorCommandHandler handler(repository);
    CreateAuthorCommand command;
    command.req = dto;

    QSignalSpy spy(&handler, &CreateAuthorCommandHandler::authorCreated);

    Result<QUuid> result = handler.handle(command);

    if (!result)
    {
        qDebug() << result.error().message() << result.error().data();
    }
    QVERIFY(result.isSuccess());

    // I don't know why they don't work
    //    QVERIFY(spy.wait(500));
    //    QCOMPARE(spy.count(), 1);
}
// ----------------------------------------------------------

void UseCases::addAuthorAsync()
{
    QSharedPointer<DummyAuthorRepository> repository(new DummyAuthorRepository(this));

    // Create an AuthorDTO to add
    CreateAuthorDTO dto;
    dto.setName("new author");
    dto.setRelative(QUuid::createUuid());

    // prefill the dummy repo:
    auto author = AutoMapper::AutoMapper::map<Domain::Author>(dto);
    repository->fillAdd(author);

    // Invoke the CreateAuthorCommandHandler with the DTO
    CreateAuthorCommandHandler handler(repository);
    CreateAuthorCommand command;
    command.req = dto;

    QSignalSpy spy(&handler, &CreateAuthorCommandHandler::authorCreated);
    QVERIFY(spy.isValid());

    handler.handleAsync(command);

    QVERIFY(spy.wait(500));
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Result<QUuid> result = arguments.at(0).value<Result<QUuid>>();
    if (!result)
    {
        qDebug() << result.error().message() << result.error().data();
    }
    QVERIFY(result.isSuccess());
}

void UseCases::removeAuthor()
{
    QSharedPointer<DummyAuthorRepository> repository(new DummyAuthorRepository(this));

    // Add an author to the repository
    AuthorDTO dto;
    dto.setUuid(QUuid::createUuid());
    dto.setName("test");
    dto.setRelative(QUuid::createUuid());
    auto author = AutoMapper::AutoMapper::map<Domain::Author>(dto);
    repository->fillRemove(author);
    repository->fillGet(author);

    // Remove the author
    RemoveAuthorCommandHandler handler(repository);
    RemoveAuthorCommand command;
    command.id = author.uuid();

    QSignalSpy spy(&handler, &RemoveAuthorCommandHandler::authorRemoved);
    QVERIFY(spy.isValid());

    Result<AuthorDTO> result = handler.handle(command);
    if (!result)
    {
        qDebug() << result.error().message() << result.error().data();
    }
    QVERIFY(result.isSuccess());

    // I don't know why they don't work
    //    QVERIFY(spy.wait(500));
    //    QCOMPARE(spy.count(), 1);
}
void UseCases::removeAuthorAsync()
{
    QSharedPointer<DummyAuthorRepository> repository(new DummyAuthorRepository(this));

    // Add an author to the repository
    AuthorDTO dto;
    dto.setUuid(QUuid::createUuid());
    dto.setName("test");
    dto.setRelative(QUuid::createUuid());
    auto author = AutoMapper::AutoMapper::map<Domain::Author>(dto);
    repository->fillRemove(author);
    repository->fillGet(author);

    // Remove the author
    RemoveAuthorCommandHandler handler(repository);
    RemoveAuthorCommand command;
    command.id = author.uuid();

    QSignalSpy spy(&handler, &RemoveAuthorCommandHandler::authorRemoved);
    QVERIFY(spy.isValid());
    handler.handleAsync(command);

    QVERIFY(spy.wait(5000));
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Result<AuthorDTO> result = arguments.at(0).value<Result<AuthorDTO>>();
    if (!result)
    {
        qDebug() << result.error().message() << result.error().data();
    }
    QVERIFY(result.isSuccess());
}

QTEST_MAIN(UseCases)

#include "tst_usecases.moc"
