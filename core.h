#ifndef CORE_H
#define CORE_H

#include <QObject>
#include "datasource.h"
#include <QList>
#include <QPoint>

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);

    void newGame();
    void restoreGame();
    bool move(unsigned int columnFrom, unsigned int rowFrom, unsigned int columnTo, int rowTo);
    const QList<QPoint> getAvailableCellsFor(unsigned int column, unsigned int row);

signals:
    void gameOverEvent();
    void gameWinEvent();
    void bornEvent(unsigned int column, unsigned int row, int type);
    void deathEvent(unsigned int column, unsigned int row);
    void updateScoreEvent(unsigned int score);
    void moveEvent(unsigned int columnFrom, unsigned int rowFtom, unsigned int columnTo, unsigned int rowTo);

private:
    bool isPossibleMove(unsigned int columnFrom, unsigned int rowFrom, unsigned int columnTo, unsigned int rowTo);
    bool faindFreeRandomCell(unsigned int &column, unsigned int &row, int &type);
    int freeCellsCount() const;
    void gameOverHandler();
    void gameWinHandler();
    bool generateNewBalls();
    bool findAndRemoveSequences();
    bool findAndRemoveHorizontalSequences();
    bool findAndRemoveVerticalSequences();
    bool removeHorizontalDiapason(unsigned int row, unsigned int firstPos, unsigned int lastPos);
    bool removeVerticalDiapason(unsigned int column, unsigned int firstPos, unsigned int lastPos);
    void addPoints();
    void clearScore();
    void notifyFieldUpdate();
    void searchFreeCells(int column, int row, char (&map)[FIELD_SIDE_LENGTH][FIELD_SIDE_LENGTH]);

    DataSource source;
};

#endif // CORE_H
