#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include "gamecell.h"
#include "constants.h"
#include <QSqlDatabase>

class DataSource : public QObject
{
    Q_OBJECT
public:
    explicit DataSource(QObject *parent = nullptr);
    ~DataSource();
    void clearGameFiled();
    unsigned int columns() const;
    unsigned int rows() const;
    char getTypeCell(unsigned int column, unsigned int row) const;
    void setTypeCell(unsigned int column, unsigned int row, int type);
    void swap(unsigned int columnFrom, int rowFrom, unsigned int columnTo, unsigned int rowTo);
    void setScore(unsigned int score);
    unsigned int getScore();
    void saveToDB();
    void loadFromDB();

private:
    void initialDB();
    void validParamsOrThrow(unsigned int columns, unsigned int rows) const;

    GameCell arrayCells2D[FIELD_SIDE_LENGTH][FIELD_SIDE_LENGTH];
    QSqlDatabase db;
    unsigned int score = 0;
};

#endif // DATASOURCE_H
