#include "datasource.h"
#include "runtimeexception.h"
#include <QSqlQuery>
#include <QSqlRecord>

DataSource::DataSource(QObject *parent)
    : QObject{parent}
{
    initialDB();
}

DataSource::~DataSource()
{
    saveToDB();
}

void DataSource::clearGameFiled()
{
    GameCell *arrayPtr = &arrayCells2D[0][0];
    for (int i = 0; i < FIELD_SIZE; i++) {
        arrayPtr[i].type = 0;
    }
}

unsigned int DataSource::columns() const
{
    return FIELD_SIDE_LENGTH;
}

unsigned int DataSource::rows() const
{
    return FIELD_SIDE_LENGTH;
}

int DataSource::getTypeCell(unsigned int column, unsigned int row) const
{
    validParamsOrThrow(column, row);
    return arrayCells2D[column][row].type;
}

void DataSource::setTypeCell(unsigned int column, unsigned int row, int type)
{
    validParamsOrThrow(column, row);
    arrayCells2D[column][row].type = type;
}

void DataSource::swap(unsigned int columnFrom, int rowFrom, unsigned int columnTo, unsigned int rowTo)
{
    validParamsOrThrow(columnFrom, rowFrom);
    validParamsOrThrow(columnTo, rowTo);

    std::swap(arrayCells2D[columnFrom][rowFrom], arrayCells2D[columnTo][rowTo]);
}

void DataSource::validParamsOrThrow(unsigned int columns, unsigned int rows) const
{
    if (columns >= FIELD_SIDE_LENGTH) {
        throw RuntimeException("DataSource::validParamsOrThrow: columns >= FIELD_SIDE_LENGTH");
    }

    if (rows >= FIELD_SIDE_LENGTH) {
        throw RuntimeException("DataSource::validParamsOrThrow: rows >= FIELD_SIDE_LENGTH");
    }
}

void DataSource::setScore(unsigned int score)
{
    this->score = score;
}

unsigned int DataSource::getScore()
{
    return score;
}

void DataSource::saveToDB()
{
    QSqlQuery query;
    query.exec("DROP TABLE game_field");
    query.exec("create table game_field (id int primary key, type int)");

    GameCell *arrayPtr = &arrayCells2D[0][0];
    for (int i = 0; i < FIELD_SIZE; i++) {
        int type = arrayPtr[i].type;
        if (type > 0) {
            query.exec("insert into game_field values(" + QString::number(i) + ", " + QString::number(type) + ")");
        }
    }

    query.exec("create table other_data (name varchar(20) primary key, val int)");
    query.exec("replace into other_data (name, val) values('score', " + QString::number(score) + ")");
}

void DataSource::loadFromDB()
{
    clearGameFiled();
    QSqlQuery query("SELECT * FROM game_field");
    int idCol = query.record().indexOf("id");
    int typeCol = query.record().indexOf("type");
    GameCell *arrayPtr = &arrayCells2D[0][0];
    while (query.next()) {
        int id = query.value(idCol).toInt();
        int type = query.value(typeCol).toInt();
        arrayPtr[id].type = type;
    }

    query.exec("SELECT val FROM other_data WHERE name = 'score'");
    if (query.next()) {
        score = query.value(0).toUInt();
    }
}

void DataSource::initialDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("game_field");
    if (!db.open()) {
        throw RuntimeException("DataSource::initialDB: !db.open()");
    }
}

