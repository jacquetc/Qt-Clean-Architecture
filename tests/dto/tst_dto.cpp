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

class DTOTest : public QObject
{
    Q_OBJECT

  public:
    DTOTest();
    ~DTOTest();

  public slots:

  private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void authorDTORetention();
    void authorDTOEqualOperator();
    void authorDTOCopy();
    // void invertedMap();

  private:
};

DTOTest::DTOTest()
{
}

DTOTest::~DTOTest()
{
}

void DTOTest::initTestCase()
{
}

void DTOTest::cleanupTestCase()
{
}

void DTOTest::init()
{
}

void DTOTest::cleanup()
{
}

// ----------------------------------------------------------

void DTOTest::authorDTORetention()
{

    //    QUuid uuid = QUuid::createUuid();
    //    QUuid relative = QUuid::createUuid();
    //    AuthorDTO dto(uuid, "e", relative);

    //    QCOMPARE(dto.uuid(), uuid);
    //    QCOMPARE(dto.relative(), relative);
    //    QCOMPARE(dto.getName(), "e");
}
// ----------------------------------------------------------

void DTOTest::authorDTOEqualOperator()
{
    //    QUuid uuid = QUuid::createUuid();
    //    QUuid relative = QUuid::createUuid();
    //    AuthorDTO dto(uuid, "e", relative);
    //    // =
    //    AuthorDTO other = dto;
    //    other.setUuid(QUuid::createUuid());
    //    QCOMPARE_NE(dto.uuid(), other.uuid());
    //    other.setName("b");
    //    QCOMPARE_NE(dto.name(), other.name());
}

// ----------------------------------------------------------

void DTOTest::authorDTOCopy()
{
    //    QUuid uuid = QUuid::createUuid();
    //    QUuid relative = QUuid::createUuid();
    //    AuthorDTO dto(uuid, "e", relative);

    //    // copy

    //    AuthorDTO copy(dto);
    //    QCOMPARE(dto.uuid(), uuid);
    //    QCOMPARE(dto.relative(), relative);
    //    QCOMPARE(dto.getName(), "e");

    //    copy.setUuid(QUuid::createUuid());
    //    QCOMPARE_NE(dto.uuid(), copy.uuid());

    //    copy.setName("b");
    //    QCOMPARE_NE(dto.name(), copy.name());
}
// ----------------------------------------------------------

// void DTOTest::invertedMap()
//{

//    QUuid uuid = QUuid::createUuid();
//    AuthorDTO dto(uuid, "e", QUuid());

//    Domain::Author author = AutoMapper::AutoMapper::map<Domain::Author>(dto);

//    QCOMPARE(author.uuid(), uuid);
//    QCOMPARE(author.getName(), "e");
//}

QTEST_MAIN(DTOTest)

#include "tst_dto.moc"
