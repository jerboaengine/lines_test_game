#ifndef AGENT_H
#define AGENT_H

#include <QObject>
#include "core.h"

class Agent : public QObject
{
    Q_OBJECT
public:
    explicit Agent(QObject *parent = nullptr);

    Q_INVOKABLE bool moveRequest(int columnFrom, int rowFrom, int columnTo, int rowTo);
    Q_INVOKABLE void newGameRequest();
    Q_INVOKABLE QList<QPoint> getAvailableCellsFor(unsigned int columnFrom, unsigned int rowFrom);

    void restoreGameRequest();

signals:
    void moveEvent(int columnFrom, int rowFtom, int columnTo, int rowTo);
    void bornEvent (int column, int row, int type);
    void deathEvent(int column, int row);
    void gameOverEvent();
    void gameWinEvent();
    void updateScoreEvent(int score);

private:
    Core gameEngine;
};

#endif // AGENT_H
