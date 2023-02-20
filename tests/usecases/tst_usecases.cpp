#include "automapper/automapper.h"
#include "dto/author/author_dto.h"
#include "dummy_author_repository.h"
#include "dummy_repositories.h"
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
    void addAuthor();
    void removeAuthor();

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

    DummyRepositories repositories;
    auto repository = new DummyAuthorRepository(this);
    repositories.append(DummyRepositories::Author, repository);

    QUuid uuid = QUuid::createUuid();
    QUuid relative = QUuid::createUuid();
    Domain::Author author(uuid, "test", relative);
    repository->fillGet(author);

    GetAuthorRequestHandler handler(&repositories);
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

void UseCases::addAuthor()
{
    DummyRepositories repositories;
    auto repository = new DummyAuthorRepository(this);
    repositories.append(DummyRepositories::Author, repository);

    // Create an AuthorDTO to add
    CreateAuthorDTO dto;
    dto.setName("new author");
    dto.setRelative(QUuid::createUuid());

    // prefill the dummy repo:
    auto author = AutoMapper::AutoMapper::map<Domain::Author>(dto);
    repository->fillAdd(author);
    repository->fillGetAll(QList<Domain::Author>() << author);

    // Invoke the CreateAuthorCommandHandler with the DTO
    CreateAuthorCommandHandler handler(&repositories);
    CreateAuthorCommand command;
    command.req = dto;
    Result<QUuid> result = handler.handle(command);
    if (!result)
    {
        qDebug() << result.error().message() << result.error().data();
    }
    QVERIFY(result.isSuccess());

    // Check that the new author was added to the repository correctly
    auto getAllResult = repository->getAll();
    QVERIFY(getAllResult.isSuccess());
    QList<Domain::Author> authors = getAllResult.value();
    QCOMPARE(authors.size(), 1);
    QCOMPARE(authors[0].getName(), dto.getName());
    QCOMPARE(authors[0].getRelative(), dto.getRelative());
}
void UseCases::removeAuthor()
{
    DummyRepositories repositories;
    auto repository = new DummyAuthorRepository(this);
    repositories.append(DummyRepositories::Author, repository);

    // Add an author to the repository
    AuthorDTO dto;
    dto.setName("test");
    dto.setRelative(QUuid::createUuid());
    auto author = AutoMapper::AutoMapper::map<Domain::Author>(dto);
    repository->fillAdd(author);
    repository->fillGetAll(QList<Domain::Author>() << author);

    // Remove the author
    RemoveAuthorCommandHandler handler(&repositories);
    RemoveAuthorCommand command;
    command.id = author.uuid();
    Result<AuthorDTO> result = handler.handle(command);
    if (!result)
    {
        qDebug() << result.error().message() << result.error().data();
    }
    QVERIFY(result.isSuccess());
}

QTEST_MAIN(UseCases)

#include "tst_usecases.moc"
