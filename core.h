#ifndef CORE_H
#define CORE_H

#include <QObject>
#include "datasource.h"

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);

    void newGame();
    void restoreGame();
    bool move(int columnFrom, int rowFrom, int columnTo, int rowTo);

signals:
    void gameOverEvent();
    void bornEvent(int column, int row, int type);
    void deathEvent(int column, int row);
    void updateScoreEvent(unsigned int score);
    void moveEvent(int columnFrom, int rowFtom, int columnTo, int rowTo);

private:
    bool isPossibleMove(int columnFrom, int rowFrom, int columnTo, int rowTo);
    bool faindFreeRandomCell(int &column, int &row, int &type);
    int freeCellsCount() const;
    void gameOverHandler();
    bool generateNewBalls();
    bool findAndRemoveSequences();
    bool findAndRemoveHorizontalSequences();
    bool findAndRemoveVerticalSequences();
    bool removeHorizontalDiapason(unsigned int row, unsigned int firstPos, unsigned int lastPos);
    bool removeVerticalDiapason(unsigned int column, unsigned int firstPos, unsigned int lastPos);
    void addPoints();
    void clearScore();
    void notifyFieldUpdate();

    DataSource source;

};

#endif // CORE_H
