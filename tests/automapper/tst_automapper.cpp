#include "author.h"
#include "automapper/automapper.h"
#include "dto/author/author_dto.h"
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QHash>
#include <QList>
#include <QTime>
#include <QVariant>
#include <QtTest>

using namespace Contracts::DTO::Author;

class AutoMapperTest : public QObject
{
    Q_OBJECT

  public:
    AutoMapperTest();
    ~AutoMapperTest();

  public slots:

  private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // void createTag();
    void basicMap();

  private:
};

AutoMapperTest::AutoMapperTest()
{
}

AutoMapperTest::~AutoMapperTest()
{
}

void AutoMapperTest::initTestCase()
{
}

void AutoMapperTest::cleanupTestCase()
{
}

void AutoMapperTest::init()
{
}

void AutoMapperTest::cleanup()
{
}

// ----------------------------------------------------------

void AutoMapperTest::basicMap()
{

    QUuid uuid = QUuid::createUuid();
    Domain::Author author(uuid, "e", QUuid());

    AuthorDTO dto = AutoMapper::AutoMapper::map<AuthorDTO>(author);

    QCOMPARE(dto.getName(), "e");
}

QTEST_MAIN(AutoMapperTest)

#include "tst_automapper.moc"
