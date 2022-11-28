#ifndef CORE_H
#define CORE_H

#include <QObject>
#include "datasource.h"

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);

public slots:
    void newGame();
    void restoreGame();
    void generateNewBalls();
    void moveBall(int columnFrom, int rowFrom, int columnTo, int rowTo);

signals:
    void gameOver();
    void newRandomBall(int column, int row, int type);
    void setBall(int column, int row, int type);
    void removeBall(int column, int row);
    void scoreUpdated(unsigned int score);

private:
    bool faindFreeRandomCell(int &column, int &row, int &type);
    int freeCellsCount() const;
    void gameOverHandler();
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
